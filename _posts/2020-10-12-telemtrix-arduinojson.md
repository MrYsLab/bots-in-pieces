---
layout: post
title: "Evaluating ArduinoJson"
date:  2020-10-10 10:00:39 -0500
categories: arduino,telemetrix,arduinojson
tags: arduino,arduinojson
---

# Why Am I Evaluating ArduinoJson?
Currently, all data transferred between the Arduino Server and Python 
Client is in binary, positionally dependent form. When receiving a serialized
data packet, the receiver must associate the command or report descriptor with the
number of bytes that comprise the packet.

Suppose we look at the client-side report_dispatch dictionary shown below.
For every expected report, the dictionary references a method that will process the report
 and the number of bytes that follow the report identifier used to process it.

```
# The report_dispatch dictionary is used to process
        # incoming report messages by looking up the report message
        # and executing its associated processing method.
        # The value following the method is the number of bytes to
        # retrieve from the deque to process the report (not including report id).
        self.report_dispatch = {}

        # To add a command to the command dispatch table, append here.
        self.report_dispatch.update({PrivateConstants.LOOP_COMMAND: [self._report_loop_data, 1]})
        self.report_dispatch.update({PrivateConstants.DEBUG_PRINT: [self._report_debug_data, 3]})
        self.report_dispatch.update({PrivateConstants.DIGITAL_REPORT: [self._digital_message, 2]})
        self.report_dispatch.update({PrivateConstants.ANALOG_REPORT: [self._analog_message, 3]})
        self.report_dispatch.update({PrivateConstants.FIRMWARE_REPORT: [self._firmware_message, 2]})
        self.report_dispatch.update({PrivateConstants.I_AM_HERE_REPORT: [self._i_am_here, 1]})
        self.report_dispatch.update({PrivateConstants.SERVO_UNAVAILABLE: [self._servo_unavailable, 1]})
        self.report_dispatch.update({PrivateConstants.I2C_READ_REPORT: [self._i2c_read_report, 1]})
        self.report_dispatch.update({PrivateConstants.I2C_TOO_FEW_BYTES_RCVD: [self._i2c_too_few, 1]})
        self.report_dispatch.update({PrivateConstants.I2C_TOO_MANY_BYTES_RCVD: [self._i2c_too_many, 1]})
        self.report_dispatch.update({PrivateConstants.SONAR_MAX_EXCEEDED: [self._sonar_max_exceeded, 0]})
        self.report_dispatch.update({PrivateConstants.SONAR_DISTANCE: [self._sonar_distance_report, 3]})

```
Using the ANALOG_REPORT as an example, we see that there are three additional bytes needed 
to be read from the serial
port after receiving the ANALOG_REPORT identifier byte.

The report as sent by the Arduino consists of the following bytes:

1. The ANALOG_REPORT identifier.
2. The pin number.
3. The high order byte of the reported value.
4. The low order bytes of the reported value.



## Advantages Of the Current Serialization Scheme

* Small packet size.
* Fast data transfer.

## Drawbacks Of The Current Serialization Scheme

* The number of bytes in a packet has to be matched on both the sender and receiver sides.
* For a packet containing a variable number of bytes, for example, the i2c_write command,
 the number of variable bytes needs to be incorporated as a data field within the packet. The command or report 
handler then reads that number of bytes from the serial port before proceeding with the packet's processing.
* The only data type represented in this scheme is a byte. When a larger data type is needed, 
the sender needs to disassemble the data into bytes, and the receiver needs to reassemble the bytes
into the desired data type.
* The data is in binary form and cannot be easily viewed with a serial monitor.

# Can [ArduinoJson](https://arduinojson.org/) Help Resolve These Drawbacks?

I wanted to explore if there was a better way to serialize
the data going across the link. [ArduinoJson](https://arduinojson.org/) was a possible solution.

ArduinoJson is a JSON library built explicitly for Arduino MCUs. JSON
allows us to describe the data in a positionally independent manner, using
key/value pairs. It also directly supports data types larger than a byte.

For example, here is a JSON representation of an i2c_write command packet.
```
{ "command": i2c_write, "address": I2C_DEVICE_ADDRESS, "bytes_to_write": [byte 1, byte2, ...] }
```
The variable portion of the data is represented 
as an array, and no additional reads are necessary to retrieve them. 
They are just part of the JSON packet that is retrieved using the key.
If JSON is a viable solution, we also no longer need to 
match packet sizes on both the client and server-side. 

## Advantages Of JSON

* Handles data types larger than a byte.
* Precludes the need to coordinate packet sizes for the server and client.
* Because the data is human-readable, the packets can be looked at with a serial monitor.

So why not just go with JSON? Because JSON packets will be larger than the binary packets.

For example, if we build a packet with the schema of:
```
{
  "count": 0,
  "type": "r",
  "device": "i2c",
  "address": 83,
  "register": 232,
  "big_data": 2048,
  "data":[1,2,3,4,5,6]
}
```
If we implement this schema using the current binary serialization method, we will end up with a 13 byte packet.
 Unlike the JSON representation, the string values would
be translated to byte values.  For example, instead of the "device" field being set to "i2c", it would
be set to some value, such as 44. 

```
// The data and types we wish to send across the link
int count = 0;
byte mtype = 1;
byte device = 44;
byte the_register = 232;
int big_data = 2048;
byte the_data[6] = {1, 2, 3, 4, 5, 6};

// prebuild the packet
byte the_buffer[13]= {highByte(count), lowByte(count), mtype, device, the_register, highByte(big_data),
lowByte(big_data), 1, 2, 3, 4, 5, 6};
```

Using the same schema with JSON, the packet length expands to 102 bytes. The ArduinoJson library
also introduces additional overhead in processing time and memory utilization.

## MessagePack - A Variant Of JSON

There is a third alternative called
 [MessagePack](https://msgpack.org/index.html) that is supported by ArduinoJson. MessagePack serializes the 
 data using compression 
techniques. For the example shown above, the length of the packet is reduced from 102 bytes to 70. 

## Drawbacks Of ArduinoJson And MessagePack

* Larger packet lengths.
* Additional time to transmit the larger packets.
* Additional processing time for serialization.
* Larger memory footprint to accommodate the library.

# So Is ArduinoJson The Answer?

The advantages of ArduinoJson over the current serialization scheme are significant,
but the question remains, do the drawbacks outweigh the benefits?

To find out, I created three test scenarios with a client and server for each scenario. The code was run
on an Arduino Uno.
You can [view the code for all three scenarios on GitHub.](https://github.com/MrYsLab/telemetrix/tree/master/arduino_json_evaluation)

They all use a similar data scheme. Because there is more likely to be a higher
volume of traffic coming from the Arduino when using Telemetrix, data will be
generated from the Arduino side only. Each test scenario generates a thousand
packets. The elapsed time for the 1000 packet transmission is calculated. 

Here are the results:

**Serialization Times**

Current Serialization:      Elapsed time = 1.1839430332183838 seconds

MessagePack Serialization:  Elapsed time = 6.156056642532349  seconds

JSON Serialization:         Elapsed time = 8.810443878173828  seconds

**Packet Lengths:**

Current Serialization:      13 bytes

MessagePack Serialization:  70 bytes

JSON Serialization:         102 bytes

**Arduino Footprint**

**Current Serialization:**
Sketch uses 1682 bytes (5%) of program storage space. Maximum is 32256 bytes.

Global variables use 200 bytes (9%) of dynamic memory, leaving 1848 bytes for local variables. Maximum is 2048 bytes.

**MessagePack Serialization:** 
Sketch uses 3576 bytes (11%) of program storage space. Maximum is 32256 bytes.

Global variables use 567 bytes (27%) of dynamic memory, leaving 1481 bytes for local variables. Maximum is 2048 bytes.

**JSON Serialization:**
Sketch uses 5478 bytes (16%) of program storage space. Maximum is 32256 bytes.

Global variables use 514 bytes (25%) of dynamic memory, leaving 1534 bytes for local variables. Maximum is 2048 bytes.

# Conclusion
There are many advantages to using JSON, but unfortunately, 
the throughput performance could not meet the Telemetrix Project's needs.  Also, its 
footprint is large when compared to the original serialization method. Therefore,
the original serialization scheme for transmitting and receiving serial data will be used.

I am exploring a variant on the current serialization scheme. In this scheme, the packet's length 
is calculated and placed in the packet's 
first byte, eliminating the need to store the number of bytes in a packet as part of a lookup table.
 If this variant
does not add additional overhead, it will be considered to be used sometime in the future. 
If adopted, a posting on this blog will be made.