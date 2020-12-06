---
layout: post
title: "Telemetrix4Esp8266 Released"
date:  2020-12-06 10:00:39 -0500
categories: arduino,telemetrix,esp8266
tags: arduino
---

As promised in the previous post, an [ESP8266 Telemetrix sketch](https://github.com/MrYsLab/Telemetrix4Esp8266)
has been released.

It is compatible with both the telemetrix and telemetrix-aio clients.

# What's Next?
I am currently working on a server and clients for the ESP32. I decided to create new 
branches for the clients because of specific  ESP32 features, such as touch pins. 
I could have put these features in the current clients but wanted to make an explicit distinction for ESP32 support.
For future releases, I am planningTelemetrix support for the Adafruit Circuit 
Playground Express, Adafruit Circuit Playground Bluefruit, and micro:bit V2. For the 
Playground Express Bluefruit and micro:bit, I plan to support both serial and BLE data transports.

Until next time, stay safe.