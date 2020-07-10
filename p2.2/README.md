# Project 2.2: CPU

This repo contains the framework code for your Project 2.2.

In this project, you will be using logisim-evolution to implement a 32-bit two-cycle processor based on RISC-V. Once you've completed this project, you'll know essentially everything you need to build a computer from scratch with nothing but transistors.

We hope this project will help you know how a CPU is built.

## PLEASE READ

The projects are part of your design project worth 2 credit points. As such they run in parallel to the actual course. So be aware that the due date for project and homework might be very close to each other! Start early and do not procrastinate.

## Download Framework

Run the following commands to pull the framework to your local repository. Please be familiar with basic operations of `git` before.

```
git remote add framework https://autolab.sist.shanghaitech.edu.cn/gitlab/cs110/p2.2_framework.git
git pull framework master:master
```

Check if you already have the following files after pulling.

```
.
├── README.md
├── alu.circ
├── autolab.txt
├── cpu.circ
├── cpu-sanity.sh
├── cpu-user.sh
├── create_test.py
├── logisim-evolution.jar
├── mem.circ
├── regfile.circ
├── run.circ
├── my_tests
│   ├── circ_files
│   │   └── ...
├── tests
│   ├── circ_files
│   │   └── ...
│   ├── input
│   └── └── ...
└──  venus-jvm-latest.jar
```
You should copy your `alu.circ` and `regfile.circ` into your Project 2.2 repository from your Project 2.1 repository and do some changes if necessary.

## Submit Code

Run the following commands to push your code to the remote repository.

```
git push origin master:master
```

Note that:
1. After finished, you should push to `origin` this time. (NOT `framework`).
2. We only grade code on the `master` branch.
3. Changes made out of `alu.circ`, `regfile.circ`, `cpu.circ` and `mem.circ` will NOT be considered by autograder.
