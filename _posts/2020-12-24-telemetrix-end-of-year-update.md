---
layout: post
title: "Adding ESP32 To The Telemetrix Family"
date:  2020-12-24 10:00:39 -0500
categories: arduino,telemetrix,esp32
tags: arduino
---

For those celebrating at this time of the year (and to those who may not be celebrating), I wish to extend my best wishes for health, happiness,
and prosperity.


This posting is just a quick update on what I have been working on. As of late, most of my efforts have been centered
around the Telemetrix Project. Support has been provided for Arduino, STM32, and ESP8266 MCUs. The latest
efforts are devoted to the ESP32. 

The ESP32 has been somewhat of a challenge. Things are somewhat non-standard as to how things like 
pin modes are represented in the ESP32 Arduino Core, which caused a little confusion but is resolved now.
I have a non-asyncio version that runs over WiFi. My next goal is to get serial over Bluetooth and BLE functioning.
Easier said than done. Bluetooth turns out to be a maze of twisty little mazes. Many of the articles I have found have 
not been very helpful, but I found what I believe is a treasure trove of information that hopefully will allow me
to make some progress. The information is located on the [techtutorialsx](https://techtutorialsx.com/) blog.

Here is a list of articles from that blog that I hope will be useful.


[ESP32 Bluetooth: Using the BTstack library](https://techtutorialsx.com/2017/07/08/esp32-bluetooth-using-the-btstack-library/
)

[ESP32 Bluetooth: Finding the device with Python and BTStack](https://techtutorialsx.com/2017/07/09/esp32-bluetooth-finding-the-device-with-python-and-btstack/
)

[ESP32 Arduino Bluetooth classic: Getting started](https://techtutorialsx.com/2018/03/04/esp32-arduino-bluetooth-classic-getting-started/
)

[ESP32 Arduino Bluetooth Classic: Setting the device name](https://techtutorialsx.com/2018/03/06/esp32-arduino-bluetooth-classic-setting-the-device-name/
)

[ESP32 Arduino: Serial communication over Bluetooth Hello World](https://techtutorialsx.com/2018/03/09/esp32-arduino-serial-communication-over-bluetoth-hello-world/)
)

[ESP32 Arduino: Getting the Bluetooth Device Address](https://techtutorialsx.com/2018/03/09/esp32-arduino-getting-the-bluetooth-device-address/
)

[ESP32 Arduino Serial over Bluetooth: Receiving data](https://techtutorialsx.com/2018/03/13/esp32-arduino-bluetooth-over-serial-receiving-data/
)

[ESP32 Arduino Bluetooth: Finding the device with Python](https://techtutorialsx.com/2018/03/26/esp32-arduino-bluetooth-finding-the-device-with-python/
)

[ESP32 Arduino Bluetooth Classic: Controlling a relay remotely](https://techtutorialsx.com/2018/04/27/esp32-arduino-bluetooth-classic-controlling-a-relay-remotely/
)

[ESP32 Arduino Serial over Bluetooth: Client connection event](https://techtutorialsx.com/2018/12/09/esp32-arduino-serial-over-bluetooth-client-connection-event/
)

[ESP32 Arduino Serial over Bluetooth: Client disconnection event](https://techtutorialsx.com/2018/12/10/esp32-arduino-serial-over-bluetooth-client-disconnection-event/)

[ESP32 Arduino Serial over Bluetooth: Get client address](https://techtutorialsx.com/2018/12/11/esp32-arduino-serial-over-bluetooth-get-client-address/)


My goal is to be able to auto-discover the ESP32 within the Telemetrix Python client without having to force the user
to do any manual steps to create the connection.

If it turns out that I cannot achieve this goal, I most likely will abandon Bluetooth and BLE support for the ESP32. 
Hopefully, I will be able to figure it out ;-).

See you in the New Year.