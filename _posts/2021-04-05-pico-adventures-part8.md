---
layout: post
comments: true
title: "Adventures With The Raspberry Pi Pico - Part 8"
date:  2021-04-05-10:00:39 -0500
categories: Raspberry Pi Pico
tags: MCU
---

# Improved PWM and Servo Support Added

## PWM Support
I have modified the API. The duty-cycle may be specified either as a percentage of 
0-99 or in a range of 0-19990 microseconds.

## Servo Support
I was able to implement servo support using standard PICO hardware_pwm SDK methods. 
When setting a pin mode to servo mode, you can specify the minimum and maximum duty cycle 
values to allow you to tune where the 0 and 180 positions are for your servo. You may 
individually tune each servo.

## What's Next
I am going to try my hand at implementing PIO support for the HC-SR04 Sonar distance 
sensor. 

Stay tuned!

