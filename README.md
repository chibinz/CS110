# Project 1: RISC-V Instruction Set Emulator

This repo contains the framework code for your Project 1.

In this project, you will create an emulator that is able to execute a subset of the RISC-V ISA. You'll provide the machinery to decode and execute a couple dozen RISC-V instructions. You're creating what is effectively a miniature version of VENUS!

We hope this project will enhance your C programming skills, familiarize you with some of the details of RISC-V, and prepare you for what's to come later in this course.

## PLEASE READ

The projects are part of your design project worth 2 credit points. As such they run in parallel to the actual course. So be aware that the due date for project and homework might be very close to each other! Start early and do not procrastinate.

## Download Framework

Run the following commands to pull the framework to your local repository. Please be familiar with basic operations of `git` before.

```
git remote add framework http://autolab.sist.shanghaitech.edu.cn/gitlab/cs110_projects/p1_framework.git
git pull framework master:master
```

You can check if you already have the following files after pulling.

```
.
├── Makefile
├── README.md
├── part1.c
├── part2.c
├── riscv.c
├── riscv.h
├── riscvcode
│   ├── code
│   │   ├── multiply.input
│   │   ├── random.input
│   │   └── simple.input
│   └── ref
│       ├── multiply.solution
│       ├── multiply.trace
│       ├── random.solution
│       ├── random.trace
│       ├── simple.solution
│       └── simple.trace
├── types.h
├── utils.c
└── utils.h
```

## Submit Code

Run the following commands to push your code to the remote repository.

```
git push origin master:master
```

Note that:
1. After finished, you should push to `origin` this time. (NOT `framework`).
2. We only grade code on the `master` branch.
3. Changes made out of `part1.c`, `part2.c`, and `utils.c` will NOT be considered by autograder.
