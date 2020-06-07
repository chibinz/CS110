# Project 4: Longan Nano minigame

This repo contains the framework code for your Project 4. 

In this project, you will be implementing a Pong game using the Longan Nano development board we've given to you. We've provided you a framework here.

You can also choose to implement your own minigame if you think your work is better than this Pong game.

We hope this project will help you experience programming RISC-V on a real machine rather than in a simulator.

For full details of this project, please see [this link](https://robotics.shanghaitech.edu.cn/courses/ca/20s/projects/4/).

## PLEASE READ

The projects are part of your design project worth 2 credit points. As such they run in parallel to the actual course. So be aware that the due date for project and homework might be very close to each other! Start early and do not procrastinate.

## Download Framework

Run the following commands to pull the framework to your local repository. Please be familiar with basic operations of `git` before.

```
git remote add framework https://autolab.sist.shanghaitech.edu.cn/gitlab/cs110/p4_framework.git
git pull framework master:master
```

Check if you already have the following files after pulling.

```
.
├── LICENSE
├── Makefile
├── README.md
├── dfu-util
├── include
│   ├── README
│   ├── fatfs
│   │   ├── diskio.h
│   │   ├── ff.h
│   │   ├── ffconf.h
│   │   └── tf_card.h
│   ├── gd32v_pjt_include.h
│   ├── gd32vf103_libopt.h
│   ├── lcd
│   │   ├── bmp.h
│   │   ├── lcd.h
│   │   └── oledfont.h
│   ├── systick.h
│   └── utils.h
├── platformio.ini
└── src
    ├── assembly
    │   └── example.S
    ├── fatfs
    │   ├── 00history.txt
    │   ├── 00readme.txt
    │   ├── ff.c
    │   ├── ffsystem.c
    │   ├── ffunicode.c
    │   └── tf_card.c
    ├── lcd
    │   └── lcd.c
    ├── main.c
    ├── systick.c
    └── utils.c
```

## Submit Code

Run the following commands to push your code to the remote repository.

```
git push origin master:master
```

Note that:
1. After finished, you should push to `origin` this time. (NOT `framework`).
2. We only grade code on the `master` branch.
