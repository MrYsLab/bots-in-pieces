---
layout: post
comments: true
title: "Introducing The Telemetrix Project"
date:  2020-09-20 11:00:39 -0500
categories: arduino,stm32,firmata
tags: arduino
---
# So, What Is Telemetrix?
Telemetrix is a play on the word "telemetry." Telemetry collects data at remote points and automatically transmits the data to 
receiving equipment (telecommunication) for monitoring. 
The [telemetrix package](https://github.com/MrYsLab/telemetrix) consists of 2 major
components, the [Python client](https://github.com/MrYsLab/telemetrix/blob/master/phase1/telemetrix/telemetrix.py) 
and the [Arduino server](https://github.com/MrYsLab/telemetrix/blob/master/phase1/arduino-telemetrix/arduino-telemetrix.ino).

If this sounds like a description of Firmata, it is no small coincidence. The telemetrix project intends to replace Arduino Firmata 
with a more straightforward protocol and an eye toward allowing feature extensibility in a simple manner.

# And, Why Telemetrix?
Creating a replacement for Firmata might seem strange from someone who has spent the better part of the last seven years creating both 
Python Firmata clients 
(pymata, pymata-aio, pymata4 pymata-express) 
and two extended versions of StandardFirmata (FirmataPlus and FirmataExpress).

What convinced me to develop telemetrix is this [issue](https://github.com/MrYsLab/pymata4/issues/17) reported against pymata4. 
 The issue is specific to STM32 processor boards and has to do with
 analog input pins number designations being incorrectly mapped to their physical pins. 
 
 If you read the issue, you will see my trials and tribulations trying to get an 
 STM32 up and running. After finding a board that I could program, I could not 
 establish serial communication between any pymata client and StandardFirmata. 
 However, I created a simple native Arduino sketch that allowed me to send a 
 character with the Arduino serial monitor and have it looped back, so I knew the board, 
 and serial communication worked. What was the cause of Firmata's inability to
 communicate over the serial link for this STM32 board? This problem is challenging to debug since the only 
 simple debug tool for Firmata is sending informational debug packets across the serial link. 
 However, since there was no communication, to begin with, 
 hunting down the exact cause would be a slow and painful process. Instead, I decided to design a replacement for Firmata.
 
# Design Requirements
My first requirement was to try and simplify things. First, I would abandon the 7-bit bytes of 
Firmata and the whole SysEx messaging scheme. All data going across the link would be 
standard 8-bit bytes, and one messaging procedure would be used, not the 2 two used by Firmata.

Next, telemetrix would use only the board descriptions built into the Arduino build infrastructure 
to provide pin mapping. Firmata, on the other hand, redefines things in its Boards.h file. 
When a new board is introduced, updating Boards.h can be very painful. 
The information in Boards.h is redundant to that provided in the board-specific Arduino core files. 
Telemetrix only uses board descriptions that are built into the board-specific Arduino core files.

# Releases In Phases
Telemetrix will be released in the following phases.

* Phase 1 Base Functionality
   * Debug capabilities
      * Serial loopback utility
      * Ability to send values from the sketch and print them on the client console.
   * Auto-detection of serial ports, including detection, locating, and connecting to a specific device.
   * Digital input
   * Digital and PWM output.
   * Analog input.
   * All inputs provide immediate notification of data changes.
   * A set of working examples for each function. Tested at a minimum on an Arduino Uno.
   
* Phase 2 Servo Functionality 

* Phase 3 I2C Functionality

* Phase 4 HC-SRO4 Distance Sensor Functionality

* Phase 5 DHT Functionality.

# Trying Out Phase 1 For Yourself

Phase 1 is available now if you wish to examine the code and try the examples. 
If you encounter any issues or have any questions, please enter an issue [here.](https://github.com/MrYsLab/telemetrix/issues) 

## Step 1 - Download the distribution.
Go to the [telemetrix repository.](https://github.com/MrYsLab/telemetrix) Click on the green button to download the code and then
unzip the file.

## Step 2 - Upload arduino-telemetrix on your Arduino
Go to the phase1/arduino-telemetrix directory to find arduino-telemetrix.ino. Upload this to
your Arduino.

## Step 3 - Try out the examples.
Go to the phase1 directory. For Windows type:

```
python3 setup.py install
``` 

or for Linux and macOS

```
sudo python3 setup.py install
```

With your Arduino connected to your computer, go to the phase1/examples directory
and try the loopback utility:

```
python3 loop_back.py
``` 

You should see A, B, and Z sent and looped back. You can then select and try out any of the other examples.
Please note that you may need to adjust the pin numbers for your specific hardware setup.

# What's Next
In the next posting, I will be explaining the code on both the Python and Arduino sides in 
some detail.


 
