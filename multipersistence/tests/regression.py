"""CLI regression suite. Only the Python standard library is needed."""
from fractions import Fraction
from itertools import combinations, permutations, product
from pathlib import Path
import json
import random
import subprocess
import sys
import tempfile

EXE = Path(sys.argv[1]).resolve()
EX = Path(__file__).resolve().parents[1] / "examples"
calls = 0

def run(*args, error=False, raw=False):
    global calls
    calls += 1
    result = subprocess.run([str(EXE), *map(str,args)], capture_output=True, text=True)
    if error:
        assert result.returncode and result.stderr.strip(), (args,result.stdout)
        return
    assert result.returncode == 0, (args,result.stderr)
    return result.stdout if raw else json.loads(result.stdout)

def span(columns):
    result = {0}
    for v in columns:
        result.update([a ^ v for a in result])
    return result

def homology_image(vertices, births, degree, sources, target):
    """Direct finite vector-set oracle in original simplex coordinates."""
    def active(s,p):
        return any(all(a<=b for a,b in zip(g,p)) for g in births[s])
    cells = [s for s in vertices if len(s)==degree+1]
    lower = [s for s in vertices if len(s)==degree]
    def boundary(s,rows):
        return sum(1<<i for i,f in enumerate(rows) if set(f).issubset(s))
    boundaries=span([boundary(s,cells) for s in vertices if len(s)==degree+2 and active(s,target)])
    common=None
    for p in sources:
        pairs={(0,0)}
        for i,s in enumerate(cells):
            if active(s,p):
                d=boundary(s,lower)
                pairs.update([(a^(1<<i),b^d) for a,b in pairs])
        image={z^b for z,d in pairs if d==0 for b in boundaries}
        common=image if common is None else common & image
    return len(common).bit_length()-len(boundaries).bit_length()

def as_grade(p):
    return ",".join(map(str,p))

def filtration_text(data):
    return "".join(f"{as_grade(s)} : {' '.join(map(as_grade,gs))}\n" for s,gs in data.items())

def single_query(file,degree,ps,q):
    return run("intersect",file,degree,as_grade(q),*map(as_grade,ps))["dimension"]

def compare_oracle(a,b):
    n=len(a)
    def h(u,v):
        return max(min(max(0,*(y-x for x,y in zip(p,q))) for q in v) for p in u)
    f=[[h(u,v) for v in b] for u in a]
    g=[[h(v,u) for u in a] for v in b]
    db=min(max(max(f[i][p[i]],g[p[i]][i]) for i in range(n)) for p in permutations(range(n)))
    # Enumerate all binary matrices; recover inverse by exhaustive image lookup.
    best=db
    for columns in product(range(1,1<<n),repeat=n):
        images={0:0}
        for i,v in enumerate(columns):
            images.update({x^v:tag^(1<<i) for x,tag in list(images.items())})
        if len(images)!=1<<n:continue
        cost=max(f[i][j] for i,col in enumerate(columns) for j in range(n) if col>>j&1)
        inverse_cost=max(g[j][i] for j in range(n) for i in range(n) if images[1<<j]>>i&1)
        best=min(best,max(cost,inverse_cost))
    return best,db

def check_exchange(matrix, prefix, steps):
    n=len(matrix)
    assert sorted(i for i,j in steps)==list(range(n)) and sorted(j for i,j in steps)==list(range(n))
    assert set(i for i,j in steps[:len(prefix)])==set(prefix)
    chosen=set(); targets=set()
    for i,j in steps:
        chosen.add(i);targets.add(j)
        for keep in (True,False):
            cols=[sum(1<<j for j in range(n) if (j in targets)==keep and matrix[j][i])
                  for i in range(n) if (i in chosen)==keep]
            assert len(span(cols))==1<<len(cols)

def main(tmp):
    a,b = EX/"slices-a.txt",EX/"slices-b.txt"
    p1,p2,p3,q = (0,1,1),(1,0,1),(1,1,0),(1,1,1)
    for file,value in ((a,1),(b,0)):
        assert single_query(file,1,[p1,p2,p3],q)==value
        for order in permutations([p1,p2,p3]):
            assert single_query(file,1,[q,*order,q],q)==value
        assert single_query(file,1,[q,q],q)==2
        assert single_query(file,1,[q,p1,q,p1],q)==1
        assert single_query(file,1,[q,p1,q],q)==1
        run("intersect",file,1,as_grade(q),"0,0,0",as_grade(p1),"2,2,2",error=True)
        for source in (p1,p2,p3):
            assert run("rank",file,1,as_grade(source),as_grade(q))["dimension"]==1
        assert run("betti",file,1,as_grade(q))["dimension"]==2
    tri=EX/"triangle.txt"
    s=run("summary",tri)
    assert (s["generators"],s["nonzeros"],s["homotopy_nonzeros"])==(9,12,1)
    for option in [(),("--grid",)]:
        assert run("betti",tri,1,"2,2,1",*option)["dimension"]==1
        assert run("rank",tri,1,"2,2,1","2,2,2",*option)["dimension"]==0
        for file in (tri,EX/"obstruction.txt",a,b):
            assert run("verify",file,*option)["acyclic_cones"]
    assert run("summary",EX/"obstruction.txt")["local_models"][0]["type"]=="filled-triangle"
    left,right=EX/"upsets-left.txt",EX/"upsets-right.txt"
    metrics=run("compare",left,right)
    assert (metrics["dI"],metrics["dB"])==("1","3")
    for i,col in enumerate(metrics["forward_columns"]):
        composed=set()
        for j in col:composed.symmetric_difference_update(metrics["backward_columns"][j])
        assert composed=={i}
    limited=run("compare",left,right,"--budget",0)
    assert limited["status"]=="bounded" and (limited["lower"],limited["upper"])==("1","3")
    matrix=EX/"basis.matrix"; mat=[[1,1,0],[1,0,1],[1,1,1]]
    for pre in [[],[0],[1,2],[0,1,2]]:
        res=run("exchange",matrix,",".join(map(str,pre)) or "-")
        assert res["status"]=="realized"
        check_exchange(mat,pre,res["steps"])
    assert run("exchange",matrix,"1","--budget",0)["status"]=="undecided"
    assert run("exchange",matrix,"0,0",error=True) is None
    file=tmp/"generated.txt"
    cloud=run("cloud",EX/"square.points",raw=True)
    file.write_text(cloud,encoding="ascii")
    assert run("betti",file,1,"1,1,0")["dimension"]==1
    assert run("rank",file,1,"1,1,0","2,1,0")["dimension"]==0
    assert run("betti",file,2,"2,0,0")["dimension"]==1
    assert run("verify",file)["acyclic_cones"]
    capped=run("cloud",EX/"square.points","--max-radius2",1,raw=True)
    file.write_text(capped,encoding="ascii")
    assert run("betti",file,1,"1,1,0")["dimension"]==1
    assert run("betti",file,2,"2,0,0")["dimension"]==0
    # More than three minimal births with a forced cycle triggers a local grid.
    support=[(1,0,0),(0,2,0),(0,0,2),(0,1,1)]
    data={(0,):support,(1,):[(0,0,0)],(2,):[(0,0,0)],(0,1):support,
          (0,2):support,(1,2):[(0,0,0)],(0,1,2):support}
    file.write_text(filtration_text(data))
    assert run("summary",file)["local_models"][0]["type"]=="grid"
    assert run("verify",file)["acyclic_cones"]
    # Independent exact queries on multicritical random complexes.
    rng=random.Random(1826)
    vertices=[(i,) for i in range(4)]
    edges=list(combinations(range(4),2))
    for _ in range(8):
        grades=list(product(range(2),repeat=3))
        data={s:[(0,0,0)] for s in vertices}
        data.update({s:rng.sample(grades,rng.randint(1,4)) for s in edges})
        data.update({s:[q] for s in combinations(range(4),3) if rng.random()<0.55})
        file.write_text(filtration_text(data))
        assert run("verify",file)["acyclic_cones"]
        ps=rng.sample(grades,3)
        for d in (0,1,2):
            expect=homology_image(data,data,d,ps,q)
            assert single_query(file,d,ps,q)==expect
    # Independent multiset-distance oracle on three summands in two varying coordinates.
    for _ in range(6):
        sides=[[[tuple(rng.randint(-2,2) for _ in range(2))+(0,) for _ in range(2)]
                 for _ in range(3)] for _ in range(2)]
        paths=[tmp/"l.txt",tmp/"r.txt"]
        for side,path in zip(sides,paths):
            path.write_text(filtration_text({(i,):gs for i,gs in enumerate(side)}))
        answer=run("compare",*paths)
        assert tuple(map(Fraction,[answer["dI"],answer["dB"]]))==compare_oracle(*sides)
    file.write_text("0 : -1/3,.125,0  -2/6,1/8,0  0,1,1\n")
    assert run("summary",file)["generators"]==1
    assert run("betti",file,0,"-1/3,1/8,0")["dimension"]==1
    assert run("betti",file,0,"-1/3,0.124999999999999999,0")["dimension"]==0
    assert run("rank",file,0,"0,2,0","0,1,0",error=True) is None
    other=tmp/"other.txt";other.write_text("a : -1/3,3/8,0\n")
    assert run("compare",file,other)["dI"]=="1/4"
    file.write_text("")
    assert run("compare",file,other)["dI"]=="infinity"
    assert run("compare",file,file)["dI"]=="0"
    assert run("betti",file,0,"0,0,0")["dimension"]==0
    for invalid in ("0,1 : 0,0,0\n","1,1 : 0,0,0\n","0 : 0/0,0,0\n","0 : 0,0,0\n0 : 1,1,1\n",
                    "0 : 1,0,0\n1 : 0,0,0\n0,1 : 0,0,0\n"):
        file.write_text(invalid)
        run("summary",file,error=True)
    file.write_text("".join(f"{i} : 0,0,0\n" for i in range(80)))
    assert run("betti",file,0,"0,0,0")["dimension"]==80
    print(f"{calls} CLI checks passed.")

if __name__=="__main__":
    with tempfile.TemporaryDirectory() as folder: main(Path(folder))
