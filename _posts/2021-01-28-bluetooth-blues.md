---
layout: post
comments: true
title: "I've Got The Bluetooth Blues"
date:  2021-01-28 11:00:39 -0500
categories: bluetooth
tags: bluetooth telemetrix
---

![]({{site.url}}/images/unnamed.png) 

I've been working on providing a Bluetooth version of Telemetrix for several devices 
since before the New Year. To put it mildly, it has been slow going and sometimes 
exasperating. For those of you who know my work, I prefer to support Linux, macOS, and 
Windows. Unfortunately, for Bluetooth, both Classic and BLE, this will not be possible. 
Windows does not seem to be supported for Python, nor for NodeJS. I should have realized
this when reading articles on developing Bluetooth - Windows is never mentioned.  No 
Windows test tools or libraries in-sight. The Python PyBluez library 
can be compiled for Windows, but that involves installing a version of Visual Studio 
to do so. I would not even think about suggesting that for my Windows users. It would 
turn into a support nightmare for the users and me.

For Bluetooth classic RFCOMM using 
PyBlueZ, I do have a working prototype. I also have a working prototype for BLE 
using the Python Bleak library for the Nordic UART Service. However, Bleak is a 
Python asyncio library, 
so it may not be everyone's
cup of tea. Bleak, which is supposed to work with Windows, 
has known Windows issues that make it a no-go on Windows for me.

So what does this boil down to?

Here is a table of devices and their intended Telemetrix support. This should work for 
both
Linux and macOS, but I have no way of testing the mac, so there are no guarantees 
mac will work (but it should).

I also cannot guarantee a timeframe. Since the ESP32 is the easiest device to work 
with, I will be completing that first, and its code will act as a template for the 
other devices. Unfortunately, I cannot predict a timeframe since Bluetooth seems
to enjoy surprising me. Stay tuned for periodic updates as progress is made.

|                Device                |   Classic Bluetooth Non-Asyncio   | BLE Asyncio With Bleak | BLE Non-Asyncio With PyBlueZ |   |
|:------------------------------------:|:---------------------------------:|:----------------------:|:----------------------------:|:-:|
| ESP32                                | Yes                               | Yes                    | To Be Investigated           |   |
| micro:bit v2                         | Device does not support this mode | Yes                    | To Be Investigated           |   |
| Adafruit CircuitPlayground Bluefruit | Device does not support this mode | Yes                    | To Be Investigated           |   |



