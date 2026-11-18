"""Check manuscript citations in the C++ code and the published reverse index."""
import argparse
import re
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PAPERS = ROOT / "english"
INDEX = ROOT / "MATHEMATICS.md"
LINK = re.compile(r"\[([^]]+)\]\(([^)\s]+\.md)#([\w-]+)\)")
ANCHOR = re.compile(r'<a id="([\w-]+)"></a>')


def manuscript_anchors():
    papers = {}
    for path in sorted(PAPERS.glob("*.md")):
        counts = Counter(ANCHOR.findall(path.read_text(encoding="utf-8")))
        if not counts or any(count != 1 for count in counts.values()):
            raise ValueError(f"Missing or duplicate anchors in {path.name}")
        papers[path.resolve()] = set(counts)
    return papers


def citations(path, papers):
    for number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        matches = list(LINK.finditer(line))
        if path.suffix in (".hpp", ".cpp") and line.count(".md#") != len(matches):
            raise ValueError(f"Malformed manuscript citation at {path.name}:{number}")
        for match in matches:
            label, relative, fragment = match.groups()
            target = (path.parent / relative).resolve()
            if fragment not in papers.get(target, ()):
                raise ValueError(f"Missing manuscript or anchor at {path.name}:{number}: {relative}#{fragment}")
            yield target, fragment, label, number


def make_index():
    papers = manuscript_anchors()
    entries = {path: [] for path in papers}
    used = {path: set() for path in papers}
    code = ROOT / "multipersistence"
    sources = sorted([*code.joinpath("src").glob("*.hpp"), *code.joinpath("src").glob("*.cpp"),
                      *code.joinpath("tests").glob("*.cpp")])
    for path in sources:
        for paper, fragment, label, number in citations(path, papers):
            source = path.relative_to(ROOT).as_posix()
            article = paper.relative_to(ROOT).as_posix()
            entries[paper].append(
                f"| [{path.name}, line {number}]({source}#L{number}) | [{label}]({article}#{fragment}) |"
            )
            used[paper].add(fragment)
    for paper, anchors in papers.items():
        missing = anchors - used[paper]
        if missing:
            raise ValueError(f"No code cites {paper.name}: {sorted(missing)}")
        list(citations(paper, papers))
    lines = ["# From C++ to the manuscripts", "",
             "Each source link opens the comment beside the computation. The adjacent link opens the "
             "corresponding definition, formula or proof. The last section covers the three query strategies.", ""]
    for paper, rows in entries.items():
        title = paper.read_text(encoding="utf-8").splitlines()[0].removeprefix("# ")
        lines.extend([f"## {title}", "", "| C++ location | Mathematical reference |", "|---|---|",
                      *rows, ""])
    return "\n".join(lines)


if __name__ == "__main__":
    args = argparse.ArgumentParser(description=__doc__)
    args.add_argument("--write", action="store_true", help="refresh the source line links after an edit")
    result = args.parse_args()
    index = make_index()
    if result.write:
        INDEX.write_text(index, encoding="utf-8")
    elif INDEX.read_text(encoding="utf-8") != index:
        raise SystemExit("Source line links changed. Run manuscript_references.py --write.")
