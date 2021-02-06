---
layout: post
comments: true
title: "Telemetrix And Telemetrix-AIO"
date:  2020-10-23 10:00:39 -0500
categories: arduino,telemetrix,stm32
tags: arduino
---
This week phase 5 of [Telemetrix](https://github.com/MrYsLab/telemetrix) 
and the new Python asyncio client, 
[Telemetrix-AIO,](https://github.com/MrYsLab/telemetrix-aio)
 were both released.
Phase 5 of Telemetrix retains the original Python API but makes some significant changes
 to both the client and the server's internals.

# Packet Length Added To Both Command and Report Packets
To simplify adding features to Telemetrix, command and reporter packets now append the packet's 
length before sending it across the serial link.
Not only does this make debugging more manageable, 
but the size of a packet no longer needs to be maintained by both the client and the server.

# Support Provided For Processors That Contain A Second i2c Port Has Been Implemented
Some processors, such as the STM32 F411CEU6, contain a second i2c port. To turn on
support for those processors, un-comment out the following line in the Telemetrix4Arduino sketch:

```
//#define SECOND_I2C_PORT
```
If you are using the second i2c port, you will need to specify which port you wish to use:

```
def set_pin_mode_i2c(self, i2c_port=0)

def i2c_read(self, address, register, number_of_bytes,
                 callback=None, i2c_port=0)

def i2c_read_restart_transmission(self, address, register,
                                      number_of_bytes,
                                      callback=None, i2c_port=0)

def _i2c_read_request(self, address, register, number_of_bytes,
                          stop_transmission=True, callback=None, i2c_port=0)

def i2c_write(self, address, args, i2c_port=0)
```
If you are using only the default i2c port, you do not need to specify an i2c port at all.

If you are using the second i2c port, the default pins used for that port are defined in the 
Telemetrix4Arduino sketch. 

```
#define SECOND_I2C_PORT_SDA PB3
#define SECOND_I2C_PORT_SCL PB10
```
These are the pin numbers for the STM32. Redefine these to the pins that are specific to your board.

# Each HC-SR04 Distance Sensor Has A Unique Callback
Previously, there was a single shared callback for all HC-SR04 sensors. You may now associate a unique
callback for each HC-SR04 in use.

# DHT Temperature Sensor Support Put On Hold
DHT sensor support will be used in an upcoming article to 
demonstrate the procedure used to add library support for both the client and the server.

# Telemetrix-AIO Available For Download

Telemetrix-AIO is a Python asyncio client with a similar API to that of the non-asyncio version.

 Why asyncio? Recently I captured a snapshot
of the download statistics from pypi for both the pymata4 (non-asyncio) and pymata-express (asyncio)
libraries, and much to my surprise, pymata-express is being downloaded at a significantly higher
 rate than pymata4.

Here are the rates as of this writing on October 22, 2020:

### pymata4:
Total downloads
10,054

Total downloads - 30 days
693

Total downloads - 7 days
232

### pymata-express
Total downloads
37,766

Total downloads - 30 days
5,594

Total downloads - 7 days
1,665

# What's Next?
I will be writing the documentation for both Telemetrix and Telemetrix-AIO. Following that,
I will be writing an article on extending both versions using the DHT sensor as an example.
When all that is complete, I will be working on an ESP-8266 version of Telemetrix and Telemetrix-AIO.