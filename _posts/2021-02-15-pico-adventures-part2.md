---
layout: post
comments: true
title: "Adventures With The Raspberry Pi Pico - Part 2"
date:  2021-02-15 10:00:39 -0500
categories: Raspberry Pi Pico
tags: MCU
---

Before beginning this discussion, I thought I would take the opportunity to 
thank Les Pounder for his Pico articles in Tom’s Hardware. If you scroll down to the 
bottom of
[this article](https://www.tomshardware.
com/news/raspberry-pi-pico-tutorials-pinout-everything-you-need-to-know),
Les provides links for some very beneficial information. I used his 
[reset button tutorial](https://www.tomshardware.com/how-to/raspberry-pi-pico-reset-button)
that makes reprogramming quick and easy.

Now on to the matter at hand. If you recall, my overall intention is to provide a
[Telemetrix](https://mryslab.github.io/telemetrix/) 
interface so that the Pico may be programmed remotely using standard Python 3 from your 
PC or Raspberry Pi.

Telemetrix is wholly dependent on having a communications link between the PC and the 
MCU. In the case of the Pico, currently, that link is a serial link running at 115200.

In the last posting, I ran a quick and dirty test to loop back a short 
data string that originated from a PC. The serial interface used was the USB connector on 
the Pico, and the library for input and output was _stdio_. The intention was to time 
how long a round trip would take for a single iteration of looping back the string.

I was curious if there might be a speed advantage in using the Pico UART library over 
the previous experiment. To use the UART library, one must connect a TTL-to-USB 
converter module to the Pico.

![]({{site.baseurl}}/images/ttl_converter.jpg)

I modified 
[uart_advanced.c](https://github.com/raspberrypi/pico-examples/blob/master/uart/uart_advanced/uart_advanced.c)
that is provided in the Pico examples directory.

I also modified the Python script I used in the previous posting to use the 
communications port assigned to the converter. What I found was surprising. 
A single round trip of 5 characters took over a second to perform. I am probably 
missing something, but I could not find any delays within the example code. 
Interestingly, if I subtract one second from each reported round trip sample, the time 
is very close to that measured for the USB cable. Again this very 
long delay that I saw is most likely ignorance on my part. I 
[documented the issue](https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=304169)
and provided the code for both the modified Pico app and Python script in the Raspberry
Pi forum. Hopefully, someone will explain the delay I am seeing.

So, what does this mean?  If my assumption is correct about some hidden 1-second delay 
and that the delay is removable, the performance between a straight USB cable 
connection versus a TTL-to-USB converter is equivalent.

Therefore, I am going to continue development using the USB cable solution.

I also tried MicroPython using the Python _std_ library to get a performance comparison. 
Outputting using _std.write_ or _print_ works fine, but using *std.rea*d seems to confuse 
the MicroPython REPL. As a result, I am abandoning my hope for a MicroPython solution 
using a USB cable. I have not tried using the UART with MicroPython.

Next, I will write the Telemetrix Python client and Pico server to control a 
digital input pin, digital output pin, PWM output mode pin, and an analog input pin. 
The API will be similar to that for the Arduino, which may be viewed 
[here.](http://htmlpreview.github.io/?https://github.com/MrYsLab/telemetrix/blob/master/html/telemetrix/index.html)

Once the client is completed, I intend to provide an asyncio version and a 
Scratch 3 extension for the Pico.

Stay tuned!

