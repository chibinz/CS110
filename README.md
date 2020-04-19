# Project 2.1: ALU and Regfile

This repo contains the framework code for your Project 2.1.

In this project, you will create an ALU that supports all the operations needed by the instructions in our ISA, and a RegFile which should be able to write to or read from these registers specified in a given RISC-V instruction. 

We hope this project will help you know how a CPU is built, and prepare you for what's to come later in this course.

## PLEASE READ

The projects are part of your design project worth 2 credit points. As such they run in parallel to the actual course. So be aware that the due date for project and homework might be very close to each other! Start early and do not procrastinate.

## Download Framework

Run the following commands to pull the framework to your local repository. Please be familiar with basic operations of `git` before.

```
git remote add framework http://autolab.sist.shanghaitech.edu.cn/gitlab/cs110/p2.1_framework.git
git pull framework master:master
```

Check if you already have the following files after pulling.

```
.
├── .gitattributes
├── README.md
├── autolab.txt
├── alu.circ
├── alu_harness.circ
├── regfile.circ
├── regfile_harness.circ
├── tests
│   ├── alu_tests
│   │   └── ...
│   ├── regfile_tests
│   │   └── ...
│   ├── reference_output
│   │   └── ...
│   ├── binary_to_hex_alu.py
│   ├── binary_to_hex_regfile.py
│   └── test.py
├── logisim-evolution.jar
└── test.sh
```

## Submit Code

Run the following commands to push your code to the remote repository.

```
git push origin master:master
```

Note that:
1. After finished, you should push to `origin` this time. (NOT `framework`).
2. We only grade code on the `master` branch.
3. Changes made out of `alu.circ` and `regfile.circ` will NOT be considered by autograder.
