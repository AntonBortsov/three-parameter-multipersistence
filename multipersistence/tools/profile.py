"""Compare two C++ header revisions on the archived 15,724-cell input (Linux)."""
import argparse
import csv
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import shutil
import subprocess
import tempfile
import time
from zipfile import ZipFile

EXPECTED = [629, 235, 261, 476, 151, 191, 219, 131]
FIELDS = ["workload", "trial", "revision", "wall_s", "build_check_s", "query_s", "peak_KiB"]

def build(folder, headers, name, compiler, profiling=False):
    flags = ["-std=c++17", "-O3", "-DNDEBUG"]
    if profiling:
        flags += ["-pg", "-g"]
    binary = folder / name
    subprocess.run([compiler, *flags, "-I", str(headers), str(folder / "native_batch.cpp"),
                    "-o", str(binary)], check=True)
    return binary

def execute(folder, binary, queries, expected):
    cmd = ["/usr/bin/time", "-f", "%M", "-o", "peak.txt", str(binary), "input3.mp", queries]
    start = time.perf_counter()
    result = subprocess.run(cmd, cwd=folder, capture_output=True, text=True, check=True)
    elapsed = time.perf_counter() - start
    data = json.loads(result.stdout)
    if data["answers"] != expected or data["generators"] != 510791:
        raise ValueError(f"Unexpected model or answers from {binary.name}: {data}")
    return [elapsed, data["build"], data["queries"], int((folder / "peak.txt").read_text())]

def flat_profile(folder, executable, label, writer):
    execute(folder, executable, "joint.txt", EXPECTED)
    profile = subprocess.check_output(["gprof", "-p", "-b", executable, folder / "gmon.out"],
                                      cwd=folder, text=True)
    for line in profile.splitlines():
        match = re.match(r"^\s*(\d+\.\d+)\s+\d+\.\d+\s+(\d+\.\d+)\s+(?:(\d+)\s+[\d.]+\s+[\d.]+\s+)?(.+)$", line)
        if match:
            percent, seconds, calls, symbol = match.groups()
            writer.writerow([label, percent, seconds, calls or "", symbol])

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("baseline_archive", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--repeats", type=int, default=5)
    parser.add_argument("--cpu-profile", action="store_true")
    args = parser.parse_args()
    if args.repeats < 1:
        parser.error("--repeats must be positive")
    core = min(os.sched_getaffinity(0))
    os.sched_setaffinity(0, {core})
    args.output.mkdir(parents=True, exist_ok=True)
    current = Path(__file__).resolve().parents[1] / "src"
    with tempfile.TemporaryDirectory(prefix="mp-profile-") as tmp:
        folder = Path(tmp)
        with ZipFile(args.baseline_archive) as z:
            for name in ["native/core.hpp", "native/model.hpp", "native_batch.cpp", "joint.txt"]:
                z.extract(name, folder)
            with z.open("inputs/15724/input3.mp") as src, (folder/"input3.mp").open("wb") as dst:
                shutil.copyfileobj(src, dst)
        (folder/"triple.txt").write_text((folder/"joint.txt").read_text().splitlines()[-1]+"\n")
        versions = {"before": folder/"native", "after": current}
        binaries = {k:build(folder,v,k,args.compiler) for k,v in versions.items()}
        # Prime the filesystem cache symmetrically; each measured run still starts a fresh process.
        for binary in binaries.values():
            execute(folder,binary,"joint.txt",EXPECTED)
        with (args.output/"timings.csv").open("w",newline="") as f:
            writer = csv.writer(f)
            writer.writerow(FIELDS)
            for trial in range(1,args.repeats+1):
                for task,query,answers in [("eight","joint.txt",EXPECTED),("triple","triple.txt",[131])]:
                    order = ["before", "after"] if (trial + (task=="eight"))%2 else ["after","before"]
                    for k in order:
                        values = execute(folder,binaries[k],query,answers)
                        writer.writerow([task,trial,k,*values])
                        f.flush()
        if args.cpu_profile:
            with (args.output/"cpu.csv").open("w",newline="") as f:
                writer=csv.writer(f)
                writer.writerow(["revision","sample_percent","self_s","calls","symbol"])
                for k,headers in versions.items():
                    exe=build(folder,headers,k+"-pg",args.compiler,profiling=True)
                    flat_profile(folder,exe,k,writer)
        def digest(path):
            return hashlib.sha256(path.read_bytes()).hexdigest()
        cpu=next(line.split(":",1)[1].strip() for line in Path("/proc/cpuinfo").read_text().splitlines() if line.startswith("model name"))
        metadata=dict(cpu=cpu,logical_cpu=core,kernel=platform.release(),
                      compiler=subprocess.check_output([args.compiler,"--version"],text=True).splitlines()[0],
                      flags=["-std=c++17","-O3","-DNDEBUG"],repeats=args.repeats,
                      input_sha256=digest(folder/"input3.mp"),harness_sha256=digest(folder/"native_batch.cpp"),
                      headers_sha256={k:{p.name:digest(p) for p in sorted(v.glob("*.hpp"))} for k,v in versions.items()},
                      generators=510791,answers=EXPECTED)
        (args.output/"run.json").write_text(json.dumps(metadata,indent=2)+"\n")

if __name__ == "__main__":
    main()
