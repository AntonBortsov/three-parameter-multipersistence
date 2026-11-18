"""Exercise lazy and tabulated queries through the production CLI."""
from itertools import combinations, combinations_with_replacement, product
from pathlib import Path
import json
import random
import tempfile
import unittest
from regression import EX, as_grade, filtration_text, homology_image, run

class Modes(unittest.TestCase):
    def setUp(self):
        tmp=tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        self.root=Path(tmp.name)
        self.src=self.root/"cells"
        self.table=self.root/"table"
        self.qs=self.root/"queries"
    def exercise(self,degree,queries,expected):
        grid=self.root/"index"
        copy=self.root/"converted"
        run("index-build",self.src,degree,grid)
        run("table-build",self.src,degree,self.table)
        run("table-from-index",grid,copy)
        self.assertEqual(copy.read_bytes(),self.table.read_bytes())
        self.qs.write_text("".join(" ".join(map(as_grade,(q,*ps)))+"\n" for q,ps in queries))
        for cmd in (("lazy-batch",self.src,degree),("table-batch",self.table),("index-batch",grid)):
            with self.subTest(mode=cmd[0],degree=degree):
                actual=[json.loads(row)["dimension"] for row in run(*cmd,self.qs,raw=True).splitlines()]
                self.assertEqual(actual,expected)

    def test_reference_vector_spaces(self):
        rng=random.Random(2207)
        grid=list(product(range(2),repeat=3))
        qs=[(q,ps) for q in grid for k in (1,2,3) for ps in combinations_with_replacement(
            [p for p in grid if all(a<=b for a,b in zip(p,q))],k)]
        rng.shuffle(qs)
        for _ in range(3):
            data={(i,):[grid[0]] for i in range(4)}
            data.update({e:rng.sample(grid,rng.randint(1,4)) for e in combinations(range(4),2)})
            data.update({s:[grid[-1]] for s in combinations(range(4),3) if rng.random()<.65})
            self.src.write_text(filtration_text(data))
            for k in (0,1,2):
                self.exercise(k,qs,[homology_image(data,data,k,ps,q) for q,ps in qs])

    def test_target_changes_and_shared_classes(self):
        arms=((0,1,1),(1,0,1),(1,1,0)); q=(1,1,1)
        qs=[(q,arms),(arms[0],[arms[0]]),(q,arms[::-1]),(q,[q,q,q]),(q,[arms[0],q,arms[0]])]
        for name,v in (("slices-a.txt",1),("slices-b.txt",0)):
            self.src.write_bytes((EX/name).read_bytes())
            self.exercise(1,qs,[v,1,v,2,1])
            self.assertEqual(run("lazy-query",self.src,1,as_grade(q),*map(as_grade,(*arms,q)))["dimension"],v)
            run("table-query",self.table,as_grade(q),*map(as_grade,(*arms,q)),error=True)

    def test_rational_empty_and_invalid(self):
        self.src.write_text("0 : -1/3,1/8,0\n")
        for empty in (False,True):
            if empty: self.src.write_text("")
            run("table-build",self.src,0,self.table)
            for cmd in (("table-query",self.table),("lazy-query",self.src,0)):
                for s,answer in (("-.2,.125,0",1),("-1/3,.1249,0",0),("-1/3,1/8,0",1)):
                    self.assertEqual(run(*cmd,"1,1,1",s,s,s)["dimension"],answer if not empty else 0)
                run(*cmd,"1,1,1","-2,0,0","2,1,1",error=True)
            self.qs.write_text("1,1,1\n")
            run("table-batch",self.table,self.qs,error=True)
            run("lazy-batch",self.src,0,self.qs,error=True)

    def test_width_and_damaged_file(self):
        for size in (257,65536):
            self.src.write_text("".join(f"{i} : 0,0,0\n" for i in range(size)))
            run("table-build",self.src,0,self.table)
            self.assertEqual(run("table-query",self.table,"1,1,1","0,0,0")["dimension"],size)
        good=self.table.read_bytes()
        for data in (good[:-1],good+b"x",b"MPTRIPL1",b"MPHGRID1"+good[8:]):
            self.table.write_bytes(data)
            run("table-query",self.table,"0,0,0","0,0,0",error=True)

if __name__=="__main__":
    unittest.main(argv=["query_modes"],verbosity=2)
