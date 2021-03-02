#!/usr/bin/python3
# trim unsupported assembly directives
import sys
d = {'.file', '.size', '.type', '.p2align', '.section', '.ident', '.addrsig'}

with open(sys.argv[1], 'r+') as f:
    lines = f.readlines()
    f.seek(0)
    for l in lines:
        if len(l.split()) > 0 and l.split()[0] not in d:
                f.write(l)
    f.truncate()
