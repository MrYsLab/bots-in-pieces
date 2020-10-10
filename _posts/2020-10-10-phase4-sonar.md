---
layout: post
title: "Telemetrix Phase 4 - HC-SRO4 Sonar Distance Sensor Support"
date:  2020-10-10 10:00:39 -0500
categories: arduino,telemetrix,hc-sr04,sonar
tags: arduino,i2c,hc-sr04,sonar
---
 
# A Slight Change Of Plan
I decided to implement support for the HC-SR04 SONAR distance sensors and not delay it to a future phase.

I've tested the current code on both an Arduino Uno and an STM32 Pro Black Pill STM32F411CEU6.

## Some Implementation Details

Up to 6 HC-SR04 sensors are supported. The distance reports for all sensors consist of a 
list containing the following:

```
[PrivateConstants.SONAR_DISTANCE, trigger_pin, distance_value, time_stamp]
```
The distance value is expressed in centimeters.

## Adding A New HC-SR04 Device
The Python _set_pin_mode_sonar_ method is used to add HC-SR04 devices. It takes three parameters, 
the trigger pin, echo pin, and a mandatory callback function.  Currently, all six HC-SR04 sensors share a
common callback function. However, this may be changed in the future so that
each sensor has its own callback. 

When the Arduino core board receives a request to add an additional HC-SR04, the _sonar_new_ function is called. 
HC-SR04 instances are identified by the trigger pin number specified for the device. 

The code for managing the HC-SR04 devices is provided below: 

```
// HC-SR04 Sonar Management
#define MAX_SONARS 6

typedef struct Sonar{
    uint8_t trigger_pin;
    Ultrasonic *usonic;
};

Sonar sonars[MAX_SONARS];

byte sonars_index = 0; // index into sonars struct

byte last_sonar_visited = 0;

unsigned long sonar_current_millis;        // for analog input loop
unsigned long sonar_previous_millis;       // for analog input loop
uint8_t sonar_scan_interval = 33 ;         // Milliseconds between sensor pings.
                                           // 29ms is about the minimum to avoid crosstalk


void sonar_new(){
    if(sonars_index == MAX_SONARS){
        byte report_message[1] = {SONAR_MAX_EXCEEDED};
        Serial.write(report_message, 1);
        return;
    }

    sonars[sonars_index].usonic = new Ultrasonic((uint8_t)command_buffer[0], 
                                                  (uint8_t)command_buffer[1],
                                                 80000UL);
    sonars[sonars_index].trigger_pin = command_buffer[0];
    sonars_index++;
}

void scan_sonars(){
    unsigned int distance;

    if(sonars_index) {
        sonar_current_millis = millis();
        if (sonar_current_millis - sonar_previous_millis > sonar_scan_interval) {
            sonar_previous_millis += sonar_scan_interval;
            distance = sonars[last_sonar_visited].usonic->read();
            byte report_message[4]  = {SONAR_DISTANCE, sonars[last_sonar_visited].trigger_pin,
                                       (byte)(distance>>8), (byte)(distance & 0xff)};
            Serial.write(report_message, 4);
            last_sonar_visited ++;
            if(last_sonar_visited == sonars_index){
                last_sonar_visited = 0;
            }
        }
    }
}


```

## Reading The HC-SRO4 Devices

The sensors are read sequentially, with 33 milliseconds between 
reading each sensor to ensure that they do not interfere with one another.

Please note that the more HC-SR04 devices
added, the longer it will be before the first sonar device is reread. If there is only
one HC-SR04 device, it will be read every 33 milliseconds, but if there are the full six 
HC-SRO4 devices enabled, a full scan of all six will take 198 milliseconds. 

# What Comes Next
I am evaluating using [ArduinoJson](https://arduinojson.org/) to serialize the data that is transmitted 
across the serial link.  The decision to use ArduinoJson will depend on the
 storage space and data transfer rate overhead added by using ArduinoJson.
[MessagePack](https://msgpack.org/index.html) with ArduinoJson will also be tested, which should lessen the overhead
used with ArduinoJson in pure JSON mode.


