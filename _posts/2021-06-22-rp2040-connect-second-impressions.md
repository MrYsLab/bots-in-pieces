---
layout: post
comments: true
title: "Second Impressions Of The Arduino Nano RP2040 Connect"
date:  2021-06-22-12:00:39 -0500
categories: Arduino RP2040
tags: MCU
---

![]({{site.url}}/images/nano_2040.jpg)


I was really bummed about my initial interaction with the Arduino 
Nano RP2040 Connect. However, once over the initial hurdle of setting 
things up for development, I was able to port 
Telemetrix to this board and blinked the board LED all 
within about an hour. I am indeed impressed and thrilled to say that Arduino did not repeat the Yun disaster.

Way to go, Arduino!

I had to remove DHT support since the DHTStable library 
does not seem to work for the RP2040. I will be testing all the
0ther Telemetrix interfaces and hope to publish a fully
working client and server for 
this board soon.
