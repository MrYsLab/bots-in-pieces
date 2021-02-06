---
layout: post
comments: true
title: "An Act Of Random Kindness"
date:  2021-02-03 10:00:39 -0500
categories: Raspberry Pi Pico
tags: MCU
---

I recently tweeted about my 
[Raspberry Pi Pico](http://127.0.0.1:4000/raspberry/pi/2021/01/28/RP2040.html) posting
and received a reply from Simon Walters (aka [@cymplecy](https://twitter.com/cymplecy)).
Simon is famously known for 
[S3GPIO](http://simplesi.net/s3gpio/), amongst other efforts in supporting physical 
computing education in the U.K.


Knowing that I have been mucking around Firmata support for Python over the past several 
years, Simon suggested adding similar support for the Pico. I lamented about not 
being able to obtain a Pico yet here in the U.S. In a private email, Simon offered to 
send 
me one, and I accepted. I can't thank Simon enough.



![]({{site.url}}/images/tweet1.png) 

As I anxiously await the Pico's arrival, I have been busy reading the 
[Raspberry Pi Pico documentation.](https://www.raspberrypi.org/documentation/pico/getting-started/)

Using the documentation as a guide, I have successfully set up development 
environments for coding the Pico, both on a Raspberry Pi4 and my Ubuntu 20.10 box.

As mentioned in the Tweet, my intention is to provide a 
[Telemetrix](https://mryslab.github.io/telemetrix/) server 
for the Pico, 
and a Telemetrix Python client capable of running on Linux, Windows, and 
macOS. 
Essentially, 
Telemetrix allows 
you to send commands from your PC to the MCU to perform normal operations such as GPIO 
and i2c control from a fixed application running on the MCU. In addition, it 
automatically monitors and reports any changes to 
input data.

Since the Pico provides both C language and Micropython SDKs,  the 
first
problem for me to solve is which SDK to use. From a pleasurable programming point of 
view, Python would be my first choice. Having done 
[similar projects](https://mryslab.github.io/pymata-cpx/) 
that required 
similar decisions to be made, my experience has been that MicroPython's efficiency for 
sending 
data across a serial link is rather sluggish. However, the RP2040, the Pico's 
processor, is not a "normal" processor. Without getting into the details, the RP2040 has 
the capability of providing optimized support for I/O communication. The first thing
that I will do is write a quick and dirty “benchmark” in both C and MicroPython to 
determine 
which to use. 
I will publish both the code and findings so that you may see how I 
chose the "winning" SDK. Let's all pull for MicroPython!

That's it for now. Stay tuned for my Pico adventures. 

And once again, thanks, Simon. Not only for the offer of the board but for the 
distraction from dealing with Bluetooth ;-).


