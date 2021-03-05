#! /usr/bin/env python3
# Trim unsupported assembly directives

import os
import sys
import subprocess
import re

erase_set = {
    # Unsupported assembler directives
    r"\.file",
    r"\.size",
    r"\.type",
    r"\.p2align",
    r"\.section",
    r"\.ident",
    r"\.addrsig",
    r"\.Lfunc_end",

    # Obtuse comments
    r"# %",
    r"#APP",
    r"#NO_APP",

    # Venus doesn't support %hi / %lo syntax. Use la instead.
    r"lui\t\w+, %hi\(\w+\)",
}

replace_dict = {
    r"lw\t(\w+), %lo\((\w+)\)\((\w+)\)": r"la\t\3, \2" + r"\n\t" + r"lw\t\1, 0(\3)",
    r"sw\t(\w+), %lo\((\w+)\)\((\w+)\)": r"la\t\3, \2" + r"\n\t" + r"sw\t\1, 0(\3)",
    r"addi\t(\w+), (\w+), %lo\((\w+)\)": r"la\t\1, \3",
    r"\.bss": r".data",
}


def trim(path):
    s = ""
    with open(path, "r+") as f:
        for l in f.readlines():
            sub = l
            for e in erase_set:
                sub = re.sub(e + ".*", "", sub)
            for r, n in replace_dict.items():
                sub = re.sub(r, n, sub)
            if ".zero" in sub:
                n = int(sub.split()[1])
                sub = re.sub(".zero.*", ".byte" + "\t" + "0 " * n, sub)
            if not sub.isspace():
                s += sub
    return s


if len(sys.argv) != 2:
    print("Usage: venusc.py <input.c>")
    exit(-1)

inp = sys.argv[1]
inp_no_ext, ext = os.path.splitext(inp)
inp_asm = inp_no_ext + ".asm"
inp_trimmed = inp_no_ext + ".S"

if ext != ".c":
    print("The input file must have extension .c!")
    exit(-1)

ret = subprocess.run([
    "clang",
    "--target=riscv32",
    "-march=rv32i",
    "-Os",
    "-S",
    "-o"
    f"{inp_asm}",
    f"{inp}",
])

if ret.returncode != 0:
    print(f"Error occurred when compiling {inp}")
    exit(-1)

with open(inp_trimmed, "w") as f:
    f.write(trim(inp_asm))
