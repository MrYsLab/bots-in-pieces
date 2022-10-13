---
layout: post
comments: true
title: "Adventures With The Raspberry Pi Pico - Part 9 "
date:  2021-04-23-10:00:39 -0500
categories: Raspberry Pi Pico
tags: MCU
---

# Alan In TelemetrixLand
## Where I fall into a rabbit  hole of my own making  and finally emerge three weeks later with a production release in hand.

![]({{site.baseurl}}/images/rabbit.png)

Approximately three weeks ago, I started work on supporting two additional sensors for 
[Telemetrix-Rpi-Pico.](https://mryslab.github.io/telemetrix-rpi-pico/) 
Namely, the HC-SR04 type SONAR distance sensor and the DHT Humidity/Temperature sensor.

Since I wanted to understand more about using the PIO features of the RP2040, I 
thought the HC-SR04 would be a good candidate for controlling and monitoring the 
device while I learn about coding the PIO. However, before I started to tackle the PIO 
code, I came across a
[PIO solution](https://github.com/GitJer/Some_RPI-Pico_stuff/tree/main/HCSR04) on 
GitHub, and much to my pleasure, it worked excellently. 

## About That Rabbit Hole
The discovered PIO solution requires using separate echo and trigger pins. I wanted to 
modify the 
code to work using a single pin connected to both echo and trigger. So I set about 
trying to figure out how to change GPIO pin direction using PIO code. To help monitor things, 
I connected a USB logic analyzer to the sensor using the original unmodified code.
I was able to see the activation of the trigger pin and the subsequent 
echo pin outputting varying periods depending upon the distance measured. So far, so good.
However, when I tried using a single pin and reversing direction, the trigger pulse 
was never generated. 

Typically, when I try working with something that I do not have a great deal of familiarity with, 
I model a solution using a tried and true method. In this case, it would be 
simply toggling the pin to generate a trigger pulse, changing GPIO pin direction, and 
then 
observing the generated echo.

However, I violated my own  tried and true development rules and jumped into PIO 
coding. After spending nearly a week playing with PIO code and continuously failing to 
see a trigger pulse, I reverted to  the tried and true. Guess what? My failure 
had nothing to do with PIO. To test, I used a US-100 device which is a 3-volt compatible 
version of the HC-SR04. When testing with simple GPIO pulses, 
it was clear the device itself was the cause of my issue and had nothing to do with my 
understanding of PIO coding. 

Perhaps if I used a pull-up resistor, I could’ve gotten 
things to work. However, since I want this library to be simple to use in the 
classroom without any additional components, I decided to abandon my one pin solution 
and reverted to using separate trigger and echo pins., 

There was an additional complication that I needed to solve before integrating the PIO 
solution into my code.
The 
original code was written in C++. 
Using 
CMAKE 
and 
integrating C and C++ files in a single project does not seem to be a trivial effort, 
so I 
needed to take the time to convert the original HC-SR04 code to C. Not a huge effort, 
but it did eat up some additional time.

What was the cost for all this stubbornness? Close to 2 weeks with very little to show 
for 
my wasted efforts. The good news? HC-SR04 is now in place.

## Adding DHT Support
I decided to use this [Pico example](https://github.com/raspberrypi/pico-examples/tree/master/gpio/dht_sensor) to support the device.

I compiled the code, hooked up my trusty DHT22, and it didn't work. I have used the 
same device for other projects and never had to use a pull-up resistor. Nevertheless, I
added the 
resistor, and the same result - a no-go. Now I tried a DHT11, and the same result.

I compared the code with an Arduino library, and it was very similar. There were some 
magic values in the code that I did not fully understand. So after 2 or 3 days
of head-scratching, I checked the GitHub 
repository against the open issues. Low and behold, there was an open issue matching 
mine. 
One of the user's solutions was to change one of the magic values, and when I tried that solution, 
all started working without connecting a pull-up resistor.  The author of
the original code, for some reason, has not incorporated this fix even though several users, 
including myself, verified that the solution worked.

## Supporting The Long Delays Required By DHT Devices
DHT devices require about 2 seconds to collect data to return to the user after being 
commanded to get a reading. The original code had incorporated a two-second sleep. Using a delay 
works fine for the example, but Telemetrix-Rpi-Pico can’t afford to block 
2 seconds since it continually monitors other inputs. In addition, I 
wanted to support more than one DHT device. 

When working with Arduino Firmata, I had to devise a rather complex strategy not to 
block the main loop yet wait for two seconds to pass. I could have implemented a 
similar scheme for the Pico, but the Pico SDK incorporates some rather excellent 
timer/alarm support.  With just a few lines of code and none of the Firmata 
complexity, I was able to perform a non-blocking wait using a  repeating timer.

## Out Of The Rabbit Hole - Some Final Thoughts
So, now version 1.0 of Telemetrix-Rpi-Pico is complete, I want to reflect on whether 
using the C SDK was worth doing or should I have gone to an Arduino compatible 
core library 
 [recently released](https://github.com/arduino/ArduinoCore-mbed) or an 
unofficial version of [a Pico  Arduino core.](https://github.com/earlephilhower/arduino-pico)

## Sticking With The C SDK
I decided to stay with the C SDK. It offers some features that the Arduino core does 
not, namely direct PIO support and features such as the repeating timer.  

A potential 
advantage of the Arduino core over the C SDK is the possibility of reusing Arduino 
libraries, but there is a 50/50 chance the library may not work with the RP2040.  Also, 
there is the 
familiarity of the Arduino API. Mixing C and C++ code within the Arduino ecosystem is 
easily done, so that is an advantage. However,
I intend to solve the mystery of doing something similar for the CMAKE world of the C SDK soon.

All in all, I am very impressed with the C SDK and the overall quality of its 
documentation. Also, the fact I can easily use the SDK with either VS Code or CLion (my 
IDE 
of choice) is  a big plus. I am not a fan of the Arduino IDE.

## What's Next?
I will implement a Python asyncio version of the client and then adapt that into a
[OneGPIO Gateway](https://mryslab.github.io/python_banyan/gpio_intro/)
for the Pico.

In addition, I still want to  create an i2c example for the PCA9685 servo controller. 

As always, stay tuned!






