---
layout: post
comments: true
title: "Adventures With The Raspberry Pi Pico - Part 5"
date:  2021-03-13-10:00:39 -0500
categories: Raspberry Pi Pico
tags: MCU
---

# NeoPixel Support Has Been Released

I have just released NeoPixel support for the 
[Pico using Telemetrix.](https://github.com/MrYsLab/telemetrix-rpi-pico) 

Here are the steps to control a NeoPixel strip.

1. First, you must set the pin mode using the
[set_pin_mode_neopixel](https://htmlpreview.github.io/?https://github.com/MrYsLab/telemetrix-rpi-pico/blob/master/html/telemetrix_rpi_pico/index.html#telemetrix_rpi_pico.telemetrix_rpi_pico.TelemetrixRpiPico.set_pin_mode_neopixel)
API method. Here you specify the GPIO pin number that the NeoPixel strip is 
   controlled with, the number of NeoPixels in your strip (up to a length of 150 is 
   supported), and RGB fill values.
   __NOTE:__ You must call this method before using any 
   of the following control methods
   
2. To set the colors for a given pixel within your strip, use the 
[neo_pixel_set_value](https://htmlpreview.github.io/?https://github.com/MrYsLab/telemetrix-rpi-pico/blob/master/html/telemetrix_rpi_pico/index.html#telemetrix_rpi_pico.telemetrix_rpi_pico.TelemetrixRpiPico.neo_pixel_set_value)
   method. Here, you specify the pixel number in the strip, the RGB values, 
   and whether you wish to show the changes immediately or wait until the
   [neopixel_show](https://htmlpreview.github.io/?https://github.com/MrYsLab/telemetrix-rpi-pico/blob/master/html/telemetrix_rpi_pico/index.html#telemetrix_rpi_pico.telemetrix_rpi_pico.TelemetrixRpiPico.neopixel_show)
   method is called.
   
3. You can turn all the pixels off by using the 
[neopixel_clear](https://htmlpreview.github.io/?https://github.com/MrYsLab/telemetrix-rpi-pico/blob/master/html/telemetrix_rpi_pico/index.html#telemetrix_rpi_pico.telemetrix_rpi_pico.TelemetrixRpiPico.neopixel_clear)
   method. Here you have the choice to affect the changes immediately or not.
   
4. The final method is
   [neopixel_fill,](https://htmlpreview.github.io/?https://github.
   com/MrYsLab/telemetrix-rpi-pico/blob/master/html/telemetrix_rpi_pico/index.html#telemetrix_rpi_pico.telemetrix_rpi_pico.TelemetrixRpiPico.neopixel_fill)
   where you may set all the pixels to the same color. Again, you may set the colors
   immediately or wait until you call neopixel_show.
   
Here is the code for a [demo](https://github.
com/MrYsLab/telemetrix-rpi-pico/blob/master/examples/neopixels2.py) 
using these methods.

![]({{site.baseurl}}/images/np_demo.png)


Watch a YouTube video of the demo [here.](https://youtu.be/IUSMdDpditc)

# What Is Coming Next

I am going to take a break from coding to write the User's Guide.

Once that is complete, I will be working on support for servo motors using a PCC9625 i2c 
servo motor board similar to the one shown below.
![]({{site.baseurl}}/images/PCA9685.png)




   
   