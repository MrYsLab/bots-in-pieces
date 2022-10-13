---
layout: post
comments: true
title: "Adventures With The Raspberry Pi Pico - Part 1"
date:  2021-02-12 10:00:39 -0500
categories: Raspberry Pi Pico
tags: MCU
---

The Pico arrived safely from across the pond. Simon Walters not only took the
time to package it up but also pre-wired headers to the board. Well-done Simon!

So, like many things in life, the Pico had some surprises in store. I made some
assumptions about the board which, turned out not to be entirely correct. You know the
old
saying about assuming, "Assuming makes an ass out of you and me." If you are not a
native English speaker, the expression is a play on the spelling of the word “assume.”

Now, back to the board. I compiled a couple of the examples from the C SDK without issue,
and
when I went to flash the code, I encountered my first "problem." The
[Getting Started Guide](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf)
 is somewhat ambiguous about how to put the board into "flash" mode. They say to press
 the BOOTSEL button but do not explicitly mention to do so before applying power to the
 Pico. There is now a
 ["quick start area"](https://www.raspberrypi.org/documentation/pico/getting-started/)
 on the RPi Pico documentation page that clearly explains this. It even incorporates
 an animated graphic. Just scroll down to the "Welcome to your Raspberry Pi Pico" section
  of the page and click on "Getting started with C/C++."

![]({{site.baseurl}}/images/pico_starting.png)

I first uploaded the "blink" example and then the "hello_usb" example, and both worked
without issue.

My next self-assigned task was to create a super-simple example to send data across
 the serial link and have it looped back using the USB connector on the Pico. It seemed 
like a no-brainer since
 "hello-usb" worked. I wanted to use the UART library to do so, but after reading the
  SDK source code, the UART library uses 2 GPIO pins for data transfer. The SDK implies
  that
  one
  would need to connect either an RPi or use a USB-to-TTL converter to use the UART
   library. My original intent was to use a USB cable between the computer and
   the Pico and have bidirectional binary data flowing between the two. If some type of
    USB-to-TTL converter is necessary, there is an extra expense and added complication.
    Something I hope to be able to avoid.

So, what to do? The "hello_usb" example uses the stdio printf function to send data
across the serial link. The low-level UART library is not available for a
USB to USB connection.  After a little bit of head-scratching, it dawned on me (sometimes I am a
 little slow on the uptake) to use other functions in the stdio library to send
 and receive data on the Pico over a USB cable. This revelation was non-obvious because 
the only
 stdio
 function mentioned in the C SDK documentation is printf. So I gave it a shot, and low and
  behold, it worked.

Here is code for the Pico side of the application. It is an adaptation of the "hello_usb"
program:


```
#include <stdio.h>
#include "pico/stdlib.h"


int main() {
    char buffer[30];
    stdio_init_all();
    while (true) {
        //printf("Hello, world!\n");
        //putchar('z');
        //sleep_ms(1000);
        fgets(buffer, 5, stdin);
        printf(buffer);
    }
    return 0;
}
```

It performs a read using fgets, and then turns around and sends the same data back using
printf.

The Python script running on my PC is as follows:

```
import serial
import time


class TPutComp:
    def __init__(self, runs):
        self.serial_port = serial.Serial('/dev/ttyACM0', 115200,
                                         timeout=1, writeTimeout=0)

        start_time = time.time()
        for i in range(runs):
            self.serial_port.write('abcde\n'.encode())
            data = self.serial_port.readline(7).decode()
            if data != 'abcde\r\n':
                raise RuntimeError
        elapsed = time.time() - start_time
        print(f'{i+1} Iterations elapsed time: {elapsed}')
        print(f'Calculated single iteration: {elapsed / runs}')


TPutComp(100)


```

This script uses the serial port to send out the string, "abcde" and waits until
that string returns the same string. The termination characters are an artifact of using
stdio on the Pico side. This sequence repeats for the number of iterations specified in
the run parameter.

Here is an output sample:

```
100 Iterations elapsed time: 0.3622300624847412
Calculated single iteration: 0.0036223006248474123

Process finished with exit code 0
```

I hoped to create a MicroPython script to compare serial throughput between C and 
MicroPython. However,
the version of MicroPython supplied for the Pico requires a USB-to-TTL
converter to support serial communication.

So, for now, a MicroPython version of Telemetrix for the Pico is on hold. I first want
to try using the USB-to-TTL converter with a modified version of the uart_modified.c
example provided with the C SDK. If there is no appreciable performance gain
between 
that and
the stdio version, I will then abandon my hopes for using MicroPython. However, if 
there is
a noticeable performance gain, I will then implement a MicroPython script using the UART
 library and run a performance comparison between the C and Python SDKs.

 Stay tuned for the results.
