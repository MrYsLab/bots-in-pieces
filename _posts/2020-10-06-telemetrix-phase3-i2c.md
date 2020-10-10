---
layout: post
title: "Phase 3 - I2C Support"
date:  2020-10-06 10:00:39 -0500
categories: arduino,telemetrix,i2c
tags: arduino,i2c
---
# I2C Support
Telemetrix Phase 3 has been released, implementing i2c support. You can view the Python3 API
[here.](https://htmlpreview.github.com/?https://github.com/MrYsLab/telemetrix/blob/master/phase3/api/index.html)

To use the i2c API, you must first call _set_pin_mode_i2c_ once within your application, independent
of how many i2c devices your application supports. Once _set_pin_mode_i2c_ is called, the _i2c read and write_ methods 
 interact with the individual i2c devices. An example i2c application for the ADXL345 accelerometer may be
  [viewed here.](https://github.com/MrYsLab/telemetrix/blob/master/examples/i2c_adxl345_accelerometer.py).
  
# Installing Phase 3

1. Go to the [telemetrix repository](https://github.com/MrYsLab/telemetrix) and click on the green 
code button to download the .zip file.

2. Extract the files into a directory of your choice. A directory, called telemetrix-master, will be created, and
it will contain all the necessary files.

3. Install the latest Arduino code by opening the Arduino IDE. From the main menu, go to *Sketch*, 
select *Include Library*, and then select *Add .ZIP Library*. Select the Telemetrix4Arduino.zip file in the 
telemetrix-master/phase3 directory. Finally, click on *File* from the IDE main menu, and select *Examples* and then
select Telemetrix4Arduino and upload the code to your device.

4. To install the phase3 Python code, cd to telemetrix-master/phase3, and then type:

```
# For Windows:

python setup.py install


# Or for macOS and Linux:


sudo python3 setup.py install
```
You are now ready to try out the examples in the telemtrix-master/examples directory using phase 3 code. 
Note that you may need to change pin numbers in the examples to match your
hardware setup. All of the examples require the use of Python 3.
  
# What's Next

## Try ArduinoJSON For Building Serial Communication Messages
I want to experiment using ArduinoJson for data transfer between the Python client and the Arduino server. 
I may also try implementing Telemetrix for the ESP-8266 performing data transfer between the client
and server over WiFi. I will be basing this off of the following links:
* [Get raw P2P Message from NodeMCU or ESP8266](https://www.youtube.com/watch?v=DeE6Ufsd2kk)
* [Getting raw TCP / IP messages from NodeMCU](https://github.com/shashank-shark/fly-nasa/blob/master/docs/raw-node-message/raw-node.md) 

## Supporting The HC-SR04 and DHT on Arduino Uno
Here is what the Arduino IDE reports when installing Telemetrix4Arduino for phase 3 on an Uno:

```
Sketch uses 7274 bytes (22%) of program storage space. Maximum is 32256 bytes.
Global variables use 1471 bytes (71%) of dynamic memory, leaving 577 bytes for local variables. Maximum is 2048 bytes.

```
Space is becoming tight.
 I may surround the code supporting these two features with #ifdefs to 
 make the build configurable by defining values at the top of the file.  If this works out well, users
can do the same for all the features and then select the supported features they want, but
depending upon the board being used, they may not be able to select all.

# Why ESP-8266 or ESP32 Support Is Important
For robotics type activities, untethered remote control is a necessity. Communicating
over TCP/IP provides a robust link and is not as fussy as a Bluetooth link.
More on this as my experiments progress.


 