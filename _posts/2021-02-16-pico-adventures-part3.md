---
layout: post
comments: true
title: "Adventures With The Raspberry Pi Pico - Part 3"
date:  2021-02-22 10:00:39 -0500
categories: Raspberry Pi Pico
tags: MCU
---

# First Beta Release

I've completed the code for the first beta release. This release includes support for:

* Digital Output
* PWM Output
* Digital Input
* Analog Input
* Auto-detection and connection of a single Pico board.
* Loopback (for serial connection debugging)

I am in the process of documenting a full User's Guide, but for those
who wish to give things a try:

## To install The Client Library:

**Linux/macOS:**

sudo pip3 install telemetrix-rpi-pico

**Windows:**

pip install telemetrix-rpi-pico


Here is a [link to the API](http://htmlpreview.github.io/?https://github.com/MrYsLab/telemetrix-rpi-pico/blob/master/html/telemetrix_rpi_pico/index.html)
Note that not all methods are implemented yet. For the methods that are not 
implemented, there is the following line on the first line of the method description:

 **NOT YET IMPLEMENTED!!!**

## To Install The Pico Server Application
1. [Download](https://github.com/MrYsLab/Telemetrix4RpiPico/raw/master/cmake-build-release/Telemetrix4RpiPico.uf2) 
   and save the .uf2 file.
2. Place the Pico in file upload mode:
   
   A. Press and hold the BOOTSEL button while applying power. 
   B. Release the BOOTSEL button.
   C. In your file explorer, you should see a new folder appear called: RPI-RP2.
   D. Drag the .uf2 file into this folder to upload the code.
   
3. [Download,](https://github.com/MrYsLab/telemetrix-rpi-pico/archive/master.zip) 
   save and uncompress the telemetrix-rpi-pico Github repository.
   
4. Open the _examples_ directory and run any of the examples. You may
modify them to suit your needs.

I will be implementing i2c support next.

Here is a screen-shot of running the 
[digital_input_pullup.py example:](https://github.com/MrYsLab/telemetrix-rpi-pico/blob/master/examples/digital_input_pullup.py)

![]({{site.baseurl}}/images/first_run.png)
