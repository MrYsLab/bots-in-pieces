---
layout: post
comments: true
title: "Banyan Bot Blue Part 4"
date:  2019-06-26 17:24:39 -0400
categories: banyan-framework
tags: banyan-gateways backplane monitor
---

# Understanding The Banyan Framework  

  
# In this edition of *Bots In Pieces*, we will:
1. Discuss the basics of the Python Banyan Framework.
2. Discuss the Banyan design philosophy.
3. Install the Python Banyan Framework On The Raspberry Pi
4. Discuss the Banyan components used for the Banyan Blue Bot project.
5. Discuss using *Tuning Parameters* to allow for maximal component reuse.

# Banyan 101 - The *Why*, *What*,  And *How*

This posting will provide a quick introduction to the Python Banyan Framework.
After reading this posting, if you would like to
learn more about the Banyan Framework, please check out [*the User's Guide.*](https://mryslab.github.io/python_banyan/)

> If you have any questions or comments about this or any other posting, please feel free to leave a
comment. There is a comment section at the bottom of each posting page. Checkout out our [*Comment Policy*.](https://mryslab.github.io/bots-in-pieces/about/)

## The *Why*

The purpose of a software framework is to help you manage the low level, mundane details of your application. 
This not only
allows you to concentrate on the problem at hand,
 but helps to simplify your coding and testing burden. Python Banyan does just that.

## The *What* 

A Banyan application consists of one or more modules known as Banyan components. Each component
when instantiated automatically, connects itself to the local network by using the Banyan message distribution
 mechanism, known as the Banyan Backplane. 

When a component wishes to share
information with other components it *publishes* a *Banyan protocol message* to the network.

A Banyan protocol message is simply a populated Python dictionary. The *keys* of the message are typically text strings,
and the associated value for each key is typically a text string or numeric value. Python Banyan also supports
Numpy data values, making it simple to share Numpy data within a Banyan application.

All protocol messages are created by you, the application developer.

An example of a Banyan protocol message might be:

```python
payload = {'command': 'move_robot', 'direction': 'forward', 'speed': 60}
```

When a component publishes a protocol message, it associates a *topic* with the 
message and both are published to the network as a single entity.

To publish a message, the component uses one of Python Banyan's built-in methods, named
*publish_payload.*

The *publish_payload* method accepts two parameters, the dictionary comprising the 
message payload and a topic string.

To publish the message described above with a topic of *control_robot*, the
method call would look like:

```python
payload = {'command': 'move_robot', 'direction': 'forward', 'speed': 60}

self.publish_payload(payload, 'control_robot')
```

To receive a protocol message, a component *subscribes* to one or more topics of interest. 
It will only receive messages that it has subscribed to - the
framework filters out the rest.

When a component receives a message, the framework calls its *incoming_message_processing* 
method. This method is built into the framework - you just need to overwrite its default
contents.

Here is the source code for this method:

```python

    def incoming_message_processing(self, topic, payload):
        """
        Override this method with a custom Banyan message processor for subscribed messages.
    
        :param topic: Message Topic string.
    
        :param payload: Message Data.
        """
        if self.external_message_processor:
            self.external_message_processor(topic, payload)
        else:
            print('this method should be overwritten in the child class', topic, payload)
```

Both the received payload, in the form of a Python dictionary, and its topic string are passed into this
 method by the framework.
Unchanged, this method will print the topic and payload to the console.

Typically all of the processing of the message is handled within this method, but you may create
an alternative method within the component to handle message processing. You do so by setting 
self.external_message_processor when you instantiate the component.
 
## This Is A Good Time To Install Python Banyan

If you are using the *Stretch* version of Raspbian and have not already updated your pip3 loader,
this would be a good time to do so. Open a terminal window and type:
```bash
sudo pip3 install -U pip
```

If you are using the *Buster* version of Raspbian, you do not have to update pip3, unless
you get an update message from [*pypi.*](https://pypi.org/)

For this project, we also need to install the 
[*boltons library*](https://github.com/mahmoud/boltons)

To install type:
```bash
sudo pip3 install boltons
```


Next to install python-banyan, type:
```bash
sudo pip3 install python-banyan
```

On *Stretch*, this should take about 10-15 minutes. On *Buster* it should take only about
2 to 3 minutes.

Once installed, you will be able to import Python Banyan into your project.

In addition to installing the library files, the *python-banyan* distribution installs 
3 command line executables: *the  backplane* the *monitor*, and the *banyan launcher (bls)*.

## The Backplane
Before starting any Banyan component, the 
[*Banyan Backplane*](https://mryslab.github.io/python_banyan/#examples_intro/#the-banyan-backplane)
 must be running.
To start the *backplane*, just type the following in your terminal:
```bash
backplane
```

You should see something like this in your terminal:
```bash
backplane

******************************************
Backplane IP address: 192.168.2.188
Subscriber Port = 43125
Publisher  Port = 43124
Loop Time = 0.001 seconds
******************************************
```

When any Banyan Component is started, it displays a heading similar to the one above.
The name of the component is displayed followed by the IP address of the Backplane.
The Subscriber and Publisher ports are IP ports used by the framework for sending and 
receiving Banyan protocol messages. The default port values may be *tuned* by using command line options when invoking the backplane.
 The *Loop Time* is the amount of time
the Banyan *event loop* waits to check for the possibility of the arrival of the next
protocol message.
This also is a tunable value.

## The Monitor
The [*monitor*](https://mryslab.github.io/python_banyan/#example5/#the-banyan-monitor)
 is a debugging tool that will display all published messages and their 
associated topics to the console. We will talk more about this later.

If you would like to try a simple Banyan application, please look at
[*this example*.](https://mryslab.github.io/python_banyan/#example1/) Since we installed python-banyan
for Python 3, please use python3 when running the example.

## The Banyan Launcher
This utility
[*launches*](https://mryslab.github.io/python_banyan/#example8/#launching-components)
 the *backplane* and *all* the components of your
Banyan application from a single command line.

## Some Additional *What*

The Banyan Framework at its lowest level uses TCP/IP to manage message communication.
Items like retransmission and buffering are handled automatically for you as part of the framework. 
Because TCP/IP is being used, messages are guaranteed to be received in the order that they were
sent. 

The Banyan Framework automatically and transparently handles both connection to the network and data encoding/decoding for
data transmission and receipt.

It is important to note that Banyan messages are not computer language specific. Versions of the
Banyan Framework are available for
[*JavaScript,*](https://github.com/MrYsLab/js-banyan) 
[*Java,*](https://github.com/MrYsLab/javabanyan)
 and [*Ruby*.](https://github.com/MrYsLab/rb_banyan) Each of these languages has an equivalent data
 structure to the Python dictionary, allowing a Banyan component written in one language to communicate
 with a component written in another language without any modification to the message or components.

## The *How*

The Python Banyan Framework is implemented as a [*Python class.*](https://github.com/MrYsLab/python_banyan/blob/master/python_banyan/banyan_base/banyan_base.py)
 To create a Banyan
component, you create a new class that inherits from the Banyan base class. In that
way, your component automatically has all the low-level functionality
built-in. 

Typically, the inherited class only has to implement or overwrite 2 methods. The first is
 the \__init__
method. You may add any initialization code that is specific to your component to this method.
The second is the *incoming_message_processing* method discussed above.

Of course, you can add any additional methods that your component may require to do its job.

# Banyan Based Applications

One way of thinking about a Banyan application is to compare it to a *factory assembly line*. 
As a Banyan message
makes its way from its origin to its destination, it is intercepted by intermediary
Banyan components. Each component provides a *value-added service* to the data. That
*value added* may be as simple as performing a calculation using the incoming data and then publishing
 the results of its calculation,
or perhaps, it may transform the data to another form that the next component in line will process. 

## Banyan Application Design Philosophy

When designing a Banyan Component, it is best to limit the scope of processing for the component.
Not only does this make the component simpler to code, but it also  makes it easier to test.
This allows you to implement a quality component quickly and with the best chance of reuse in the
future.

Other than the requirement that you derive your Banyan component from the
Banyan base class, there are really no other restrictions when using the framework.
The Banyan Framework is extremely flexible and does not limit your design in any way.
For example, although not often needed, you can incorporate multi-threading into a Banyan component or even asyncio.
Banyan also integrates well with other event loop driven frameworks, such as *tkinter*.

# The Banyan Bot Blue Components

The Banyan Bot application is comprised of 3 Banyan components:

1. The Bluetooth Gateway
2. The Robot Controller
3. The Hardware Control Gateway. Specifically for this application, the Hardware Control Gateway 
communicates with and controls
an [*Adafruit Crickit controller.*](https://www.adafruit.com/product/3957)
If we decide at a later time to swap out the Crickit controller for some other controller, the
Bluetooth Gateway and Robot Controller will remain unchanged. This allows for maximum flexibility
in design with minimal recoding.


## The Banyan BlueTooth Gateway

This Banyan BlueTooth Gateway is a *Banyan Protocol Gateway*. The function of a Banyan Protocol
 Gateway is to convert data to and from some protocol other than the Banyan messaging protocol.
  For this project, we need to convert Bluetooth data into Banyan protocol messages. Another example
  of a Banyan Protocol Gateway is the 
  [*MQTT Gateway.*](https://mryslab.github.io/python_banyan/#additional_gateways/#mqtt-gateway)
  
  ![]({{site.url}}/images/banyan-bot-blue-1/bluetooth_gateway.png)

  
  In the case of the Bluetooth Gateway, 
its function is to act as a Bluetooth server, and when Bluetooth data is received over the
RFCOMM link, it converts that data to a Banyan protocol message. The Bluetooth Gateway
also has the capability of subscribing to Banyan protocol messages,
 extracting the data from the message and transmitting
that data over the RFCOMM link.



## The Robot Controller

The Robot Controller subscribes to receive *generic* robot control commands. It
translates and publishes those commands as protocol messages that the hardware control component can
use to control the robot. It also subscribes to receive any report
data that the hardware control component publishes, such as bumper
sensor data. It uses that data to autonomously affect robot motion. It also publishes the report
data so that the user may see the report on their display.

![]({{site.url}}/images/banyan-bot-blue-1/robotic_gateway.png)


> Note that none of the components are actually directly aware of any of the other components. 
A component has no idea which other component published a message it subscribes to and has no idea
what other components make use of the messages it publishes. 


## The Hardware Controller (Crickit Controller)

The Cricket Controller is an instance of a *Banyan Hardware Gateway*.

![]({{site.url}}/images/banyan-bot-blue-1/crickit_gateway.png)


Banyan Hardware Gateways conform to the [*Banyan OneGPIO Protocol Messaging
Specification.*](https://mryslab.github.io/python_banyan/#gpio_intro/)

Simply put, OneGPIO protocol messages describe hardware interactions using
a high-level abstraction that is not specific to any one hardware platform.

The Hardware Gateway translates the generic OneGPIO messages into messages that are
hardware specific. It also accepts report messages generated by the hardware and
translates these reports into a Banyan protocol message and then publishes them.

Using OneGPIO, we can easily swap out one hardware control platform for another, just
by changing the Banyan Hardware Gateway without having to modify any of the other components
within the application.


# Tuning Banyan Components

As stated previously, one of the main reasons to use Banyan is because
it offers the opportunity for maximum code reuse. This means writing the
code once and being able to use it in different application settings.

Depending upon the application, you may need to change some default
values for the component
or you may need to change the component's *interface.*

We accomplish this by using the
[*Python argparse library.*](https://realpython.com/command-line-interfaces-python-argparse/)

The use of argparse is totally optional but all of the Banyan Bot Blue
components take advantage of argparse.

Argparse allows us to pass in command line options to affect the behavior
and configuration of a component.

One command line option that argparse automatically generates for us is the
*-h* option. The *-h* option prints a list of all the command line parameters
available for the component.

In most cases, the default values can be used and so no command line options
are necessary, but if we need to tune those values, we can do so easily
without having to edit the code.

Below is the
help screen for the robot_control component.

```bash
pi@BanyanBot:~/banyan-bot-blue/banyan_assets $ python3 robot_control.py -h
usage: robot_control.py [-h] [-b BACK_PLANE_IP_ADDRESS]
                        [-d PUBLISH_TO_HARDWARE_TOPIC]
                        [-l ADDITIONAL_SUBSCRIBER_LIST [ADDITIONAL_SUBSCRIBER_LIST ...]]
                        [-n PROCESS_NAME] [-p PUBLISHER_PORT]
                        [-r PUBLISH_TO_UI_TOPIC] [-s SUBSCRIBER_PORT]
                        [-t LOOP_TIME] [-u SUBSCRIBE_FROM_UI_TOPIC]
                        [-v SUBSCRIBE_FROM_HARDWARE_TOPIC]

optional arguments:
  -h, --help            show this help message and exit
  -b BACK_PLANE_IP_ADDRESS
                        None or IP address used by Back Plane
  -d PUBLISH_TO_HARDWARE_TOPIC
                        Publishing topic for hardware commands
  -l ADDITIONAL_SUBSCRIBER_LIST [ADDITIONAL_SUBSCRIBER_LIST ...]
                        Banyan topics space delimited: topic1 topic2 topic3
  -n PROCESS_NAME       Set process name in banner
  -p PUBLISHER_PORT     Publisher IP port
  -r PUBLISH_TO_UI_TOPIC
                        Publishing topic for report messages
  -s SUBSCRIBER_PORT    Subscriber IP port
  -t LOOP_TIME          Event Loop Timer in seconds
  -u SUBSCRIBE_FROM_UI_TOPIC
                        Topic From User Interface
  -v SUBSCRIBE_FROM_HARDWARE_TOPIC
                        Topic From Hardware
```

As you can see this component has many options that we can use to tune the component.
We can manually set the backplane IP address, or change default publishing or subscription
topics or change several other parameters within the component.

This gives the component optimal flexibility and allows for maximum reuse.

More on these parameters when we address using each component in future posts.

# Preview Of The Next Post
In the next post, we will demonstrate testing and using the Banyan Bot Blue components.
We will be using the *monitor* to help us test and we will use the *Banyan Launcher*
to start our components. See you next time. 


