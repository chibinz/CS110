# Lab 11 Starter

Starter files for Lab 11.

For details, Please refer to webpage [here](https://robotics.shanghaitech.edu.cn/courses/ca/20s/labs/11/).

---

# FAQ

## Setup

1. How can I get the stuff needed for this lab?

* We will give you 1 board, 2 buttons and 4 halves of Dupont cables during the lab time. Still, you need to bring your own type-C cable that can be properly connected to your computer.

2. I'm using Windows/MacOS, what should I do to get prepared?

* If you're using MacOS, the only solution is to have a ubuntu virtual machine. Sadly, the chip (gd32vf103) does not have any official support for MacOS.

* If you're using Windows, it's highly recommended to have a dual boot of any ubuntu distribution. A virtual machine will also be fine. You can also follow the guide on the official website [here](https://longan.sipeed.com/), but it's not recommended.

3. I failed to install python3 or pip.

* Please refer to the official documentation here. [This](https://www.python.org/downloads/release/python-383/) is for python3 and [this](https://pip.pypa.io/en/stable/installing/) is for python3-pip.

## PlatformIO Related Problems

4. When I run `python -m platformio run`, it raises an error: `sh: 1: dfu-suffix: not found`

* Run `sudo apt-get install dfu-util` to solve this issue. Also, this will not affect the `dfu-util` in our folder.

5. When I run `python -m platformio run`, it raises an error: `PermissionError: [Errno 13] Permission denied: .....`

* Run `sudo chmod -R 777 ~/.platformio`.

## DFU Problems

6. When I used `./dfu-util -l | grep 28e9:0189`, it raises an error: `bash: ./dfu-util: Permission denied`

* `chmod +x ./dfu-util`.

7. When I run `./dfu-util -l | grep 28e9:0189`, it shows `Cannot open device 28e9:0189`

* One option is to use `sudo`. But it will require you to use `sudo` in **all further operations** of `dfu-util` (even including `Makefile`).

* Another option is to create a file: `/etc/udev/rules.d/99-platformio-udev.rules` whose content is this line:
```
ATTRS{idVendor}=="28e9", ATTRS{idProduct}=="0189", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1", ENV{ID_MM_PORT_IGNORE}="1"
```
and restart your terminal.

8. The `dfu-util` did not find my device!

* Please make sure your device has entered DFU mode. You'll have to redo the operations needed to enter DFU mode after you've pressed the RESET button.

* Also, although it seems to be a very rare case, it is possible that your board could exit DFU mode after ~10s of your operation, which should not happen. You may do the operation and run `./dfu-util -l` very quickly and repeatedly, to see if you've had this problem.

* If this still not works, check if your Type-C cable is connected to your PC tightly. Notice that some types of Type-C cable can only be used for charging; thay can not transfer data.

## Makefile related problems

9. What's the relationship of `Makefile`, `dfu-util` and `PlatformIO`?

* The `Makefile` is actually a shorthand which only acts like a sh script here. You can know this by reading the file.

10. When I run `make`, it always terminate with an error.

* If the following is your case:
```
./dfu-util -l | grep 28e9:0189
Makefile:7: recipe for target 'download' failed
make: *** [download] Error 1
```
This means your device did not enter the dfu mode successfully. See problem 7 nd 8.

* If a prograss bar shows, and it terminates with `Error 74`, then it does not matter. You can press the RESET button if the screen is still frozen.

## Problems with Assembly Code

11. When I run `make`, it shows something like `unrecognized instruction`.

* Please only run `make` in the root folder. Also, make sure your assembly code is valid. You may need to change `.asciiz` to `.string`.

12. I wrote something, but it did not work as intended. (e.g. I wrote something to display 'abcd' on the screen, but only 'abc' is shown)

* I'm sincerely sorry for that but I always cannot reproduce the same problem on my laptop. 
* If your problem is related to **character displaying**, you may try to modify `src/lcd.c` and add this line between line 526 and 527 (which is the beginning of function `void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode,u16 color)`:) 
```
mode = 1;
```
* Also, perhaps your display color is too dark, or maybe the redius or size for a circle/rectangle is too small. Please refer to problem 13.

## Others

13. What's the parameters of the attached screen?

* Size: 160x80
* Top-left point is (0, 0), while bottom-right is (159, 79)
* Color setting: RGB565 (see [this link](https://en.wikipedia.org/wiki/High_color#16-bit_high_color) for description, and [this link](http://www.barth-dev.de/online/rgb565-color-picker/) for a color picker.)

