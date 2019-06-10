---
layout: post
title: "Banyan Bot Blue: Part 1"
date:  2019-06-10 16:24:39 -0400
categories: Banyan-Bot-Blue python-banyan
---

Over the next several postings, we will be constructing the first generation
Banyan Bot Robot. This version will be
Raspberry Pi based and remotely controlled using an Android Bluetooth device. 

Yes I know, that has been
done a million times before, but the robot is not my main motivation for this project.

My main motivation is to demonstrate how to create reusable software components. That is software
components that will act as off-the-shelf entities to be reused, without modification in future projects.

So for example, if I wish to swap out the Raspberry Pi for an ESP-8266, 
I should be able to reuse most of the components created for this project. Just plug them
in and they should be good to go!

To accomplish the goal of *reusablity*, we will be using a software
framework called [*Python Banyan*](https://mryslab.github.io/python_banyan/#). Python Banyan allows one to create small, targeted
*components* that communicate with each other over a LAN based *software backplane*.
And as the name implies, this framework is implemented in Python.

> [*JavaScript*](https://github.com/MrYsLab/js-banyan), [*Ruby*](https://github.com/MrYsLab/rb_banyan), and [*Java*](https://github.com/MrYsLab/javabanyan) Programmers:
>
> Banyan is also available for these languages as well. 
> Any component written in any of the supported languages is compatible with
components written in any of the other languages. So if Python is not your language of choice,
anything presented here in Python is still available to you for reuse.

## Advantages Of Using A Software Framework

Software frameworks allow you to focus on the problem at hand by taking care of the low-level details. Banyan,
 no matter the language implementation, handles all of the
networking and inter-component communication details. In fact, when you first instantiate
a Banyan component, it will automatically plug itself into the software backplane without you having
to remember to manage that.

### A High-Level View Of Python Banyan

Python Banyan implements and takes advantage of a software pattern known as 
the *publish/subscribe* model. Banyan is built on top of the [*ZMQ Messaging Library*](http://zeromq.org/)

A Banyan component may at its option, publish, or transmit data over the LAN. Also, any component can elect to receive
data by requesting to receive it. The data, known as a *payload*, is transmitted in the form of Python dictionary.
When a payload is published, a *topic* is associated with the payload
and published as well.
To receive data, a component *subscribes* to one or more topics. The payloads, and only the
requested payloads, 
are placed on the subscriber's receive queue to process.

![]({{site.url}}/images/banyan-bot-blue-1/Banyan1.jpg)

All of the queuing and routing is handled by the framework, again freeing you from
low-level details. The framework guarantees that messages will be received in 
the order that they are sent and handles all encoding for transmission across the network.

We will get into more detail about Python Banyan in a future post.


## Python In The House
I have selected Python as the language of choice for this project (and probably those in the future 
as well). So why Python? Well, one reason is that a [*recent article*](https://www.zdnet.com/article/programming-languages-python-predicted-to-overtake-c-and-java-in-next-4-years/)
predicts that Python will overtake both Java and C in 4 years.

When selecting a language for use, I like to consider the following:

1. The amount of quality code that can be written in a day.
2. The quality of its native library.
3. The availability of a large choice of additional open source libraries and frameworks.
4. The quality and ease of access to help.
5. The language's portability across multiple computing platforms without the need for recompilation.
6. And most important to me,  "Do I have a smile on my face at the end of the day or a frustration
frown?"

For me, Python checks all the boxes.

## Hardware Components Used In This Project

* An Android cell phone or tablet (Kindle Fire Tablets work nicely here). 
* Raspberry Pi 3B+
* [Adafruit Crickit Hat](https://www.adafruit.com/product/3957) For The Raspberry Pi (motor control and sensor input).
* A 2 motor, 5 Volt, robot chassis. Here are some links to examples of this type of chassis:
  * [From Amazon](https://smile.amazon.com/Emgreat%C2%AE-Chassis-Encoder-wheels-Battery/dp/B00GLO5SMY/ref=sr_1_17?keywords=robot+chassis&qid=1560195956&s=gateway&sr=8-17_)
  * [From Pololu](https://www.pololu.com/product/3500)
  * [From Adafruit](https://www.adafruit.com/product/3216)
  * I will be reusing [a chassis from a previous project](https://www.sparkfun.com/products/12649)
  
  
## Software Components

Below is a Banyan component diagram for this project. With the exception of the cell phone app, all
software components will be resident on the Raspberry Pi.

![]({{site.url}}/images/banyan-bot-blue-1/bbb-data-flow.jpg)

**The Components:**

* External (Non-Banyan) Components:
  * A Bluetooth Android app to control the rover's motion and that has the ability to receive and report status information from 
the rover.
* Banyan Components:
  * Bluetooth Gateway
    * Acts as a Bluetooth RFCOMM server for the Android device.
    * Converts data received from the Android device and publishes that data as a Banyan message.
    * Converts Banyan report messages into a form that the Android app can process and transmits those messages over the Bluetooth link.
  * Robotics Gateway
    * Converts message data originated from the Android device and published by the Bluetooth Gateway into Banyan motion control messages.
    * Converts and relays Banyan messages from the Raspberry Pi to the Android device.
  * Cricket Gateway
    * Receives Banyan motion messages from the Robotics Gateway and translates these into Cricket API commands.
    * Receives sensor updates from the Cricket and translates and publishes this data as Banyan messages.

Each component will be discussed in detail in future posts.

## Next Time: Coding the Android App





