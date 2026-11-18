"""Compare the persisted grid with an independent, finite-vector-space oracle."""
from itertools import combinations, combinations_with_replacement, product
from pathlib import Path
import json
import random
import tempfile
import unittest
from regression import EX, as_grade, filtration_text, homology_image, run


class GridIndex(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        root = Path(self.temp.name)
        self.file, self.index, self.requests = [root / name for name in ("input", "index", "queries")]

    def batch(self, qs):
        self.requests.write_text("".join(" ".join(map(as_grade, (q,*ps)))+"\n" for q,ps in qs))
        return [json.loads(s)["dimension"] for s in run("index-batch",self.index,self.requests,raw=True).splitlines()]

    def test_all_triples_and_targets(self):
        rng=random.Random(8029)
        grades=list(product(range(2),repeat=3))
        qs=[(q,ps) for q in grades
            for ps in combinations_with_replacement([p for p in grades if all(a<=b for a,b in zip(p,q))],3)]
        for _ in range(4):
            data={(i,):[grades[0]] for i in range(4)}
            data.update({e:rng.sample(grades,rng.randint(1,4)) for e in combinations(range(4),2)})
            data.update({s:[grades[-1]] for s in combinations(range(4),3) if rng.random()<0.6})
            self.file.write_text(filtration_text(data))
            for degree in (0,1,2):
                run("index-build",self.file,degree,self.index)
                expected=[homology_image(data,data,degree,ps,q) for q,ps in qs]
                self.assertEqual(self.batch(qs),expected)

    def test_shared_and_distinct_images(self):
        arms=((0,1,1),(1,0,1),(1,1,0))
        top=(1,1,1)
        for name, triple in (("slices-a.txt",1),("slices-b.txt",0)):
            run("index-build",EX/name,1,self.index)
            queries=[(top,[p]) for p in arms]+[(top,arms),(top,arms[::-1]),(top,[top,*arms,top])]
            self.assertEqual(self.batch(queries),[1,1,1,triple,triple,triple])

    def test_grade_boundaries_and_empty_complex(self):
        self.file.write_text("0 : -1/3,1/8,0\n")
        run("index-build",self.file,0,self.index)
        for g,ans in (("-1/3,.125,0",1), ("99,99,99",1), ("0,.1249,0",0), ("-1/2,1,1",0)):
            self.assertEqual(run("index-query",self.index,g,g)["dimension"],ans)
        run("index-query",self.index,"1,1,1","-2,0,0","2,0,0",error=True)
        self.file.write_text("")
        self.assertEqual(run("index-build",self.file,0,self.index)["nodes"],0)
        self.assertEqual(run("index-query",self.index,"1,1,1","0,0,0")["dimension"],0)

    def test_portable_file_and_invalid_inputs(self):
        run("index-build",EX/"triangle.txt",1,self.index)
        original=self.index.read_bytes()
        run("index-build",EX/"triangle.txt",1,self.index)
        self.assertEqual(self.index.read_bytes(),original)
        self.file.write_text("0,1 : 0,0,0\n")
        run("index-build",self.file,1,self.index,error=True)
        self.assertEqual(self.index.read_bytes(),original)
        self.requests.write_text("1,1,1\n")
        run("index-batch",self.index,self.requests,error=True)
        for broken in (original[:-1],original+b"x",original[:12]+bytes([original[12]^1])+original[13:],b""):
            self.index.write_bytes(broken)
            run("index-query",self.index,"1,1,1","0,0,0",error=True)
        run("index-build",EX/"triangle.txt",5,self.index,error=True)

    def test_higher_homology(self):
        top=(1,1,1)
        data={s:[(0,0,0)] for dim in range(1,4) for s in combinations(range(4),dim)}
        self.file.write_text(filtration_text(data))
        for degree,expected in ((0,1),(1,0),(2,1),(3,0),(4,0)):
            run("index-build",self.file,degree,self.index)
            self.assertEqual(self.batch([(top,[(0,0,0),top,top])]),[expected])

if __name__=="__main__":
    unittest.main(argv=["index_regression"],verbosity=2)
