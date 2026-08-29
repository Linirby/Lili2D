#!/usr/bin/env python3
import re
import sys

if len(sys.argv) > 1:
    with open(sys.argv[1], "r", encoding="utf-8") as f:
        content = f.read()

    def repl(m: re.Match[str]) -> str:
        return f"\n\\htmlonly\n{m.group(0)}\n\\endhtmlonly\n"

    new_content = re.sub(r"<video[\s\S]*?</video>", repl, content)
    _ = sys.stdout.write(new_content)
