---
layout: post
comments: true
title: "Bayan Bot Blue 2 - Explorer Edition"
date:  2019-09-08 11:00:39 -0500
categories: banyan-components robot-control
tags: 
---

# Replacing The Crickit With The Explorer Pro   
{: style="color:rgb(128,0,0); " }
![]({{site.url}}/images/banyan-bot-blue-1/exp_up.png) 
{: style="float:left ; "}
 
 <br /> <br /> <br /> <br /> <br /> <br /> <br />
<br /> <br /> <br /> <br /> <br />

# In this edition:     
* We substitute the Pimoroni Explorer Pro for the Crickit HAT and
  discuss:
  *    The experiences of performing the substitution.
  *    Creating a Banyan Hardware Gateway for the Explorer Pro.
  *    Changes needed in the other Banyan components.
  *    Controller board recommendation.
    
# Replacement Experiences
First,
the robot had to be rewired to meet the physical needs of the Explorer
Pro. Then, a new Banyan Hardware Gateway module was created to
support the Explorer Pro Python library. Both modifications were relatively simple and
straight forward.

## Battery Experience
The Explorer Pro was easily powered using 4 NiMH AA cells. I left both
motors running continuously for 10 minutes, and the batteries had more
to give. The Crickit HAT, on the other hand, lasted less than two
minutes without the motors running using the same batteries.

## Rewiring Experiences
The Explorer Pro, from a physical design point of view, is in my opinion
not as convenient as that of the Crickit. The Crickit offers +5 and
ground pins for each of its inputs and outputs and provides for screw
terminals for its motor connections. The Explorer Pro does not offer
screw terminals, but in all fairness, the motor leads never dislodged.
Both +5 and ground had to be broken out to the included mini-breadboard.
Not a significant problem, but this increased chances for a wiring error
and a somewhat physically uglier outcome. Certainly not a deal-breaker.

## Creating an Explorer Pro Banyan Gateway
Creating
[Explorer Pro Gateway, ](https://github.com/MrYsLab/bots-in-pieces-examples/blob/master/banyan-bot-blue/banyan_assets/exp_pro_gateway.py)
was not particularly challenging and followed the pattern of a typical
[Banyan Hardware Gateway.](https://mryslab.github.io/python_banyan/#banyan_gateways/)
It takes advantage of the 
[Python library supplied by Pimoroni.](https://github.com/pimoroni/explorer-hat)

### Explorer Pro Capabilities
Here is a list of capabilities of the Explorer Pro HAT. These far exceed
the needs of the project, but the gateway supports all of these
features. All were tested and worked flawlessly.

* Four buffered 5V tolerant inputs. 
* Four powered 5V outputs (up to 500mA total across all four channels).
* Four capacitive touchpads.
* Four capacitive crocodile clip pads. 
* Four LEDs. 
* Four analog inputs.
* Two H-bridge motor drivers (up to 200mA per channel; soft PWM control)

[A Tkinter GUI](https://github.com/MrYsLab/bots-in-pieces-examples/blob/master/banyan-bot-blue/test_fixtures/exp_pro_gui.py) is provided to aid in testing.


### Handling "Chatty" Analog Inputs And Testing Anomalies
The gateway supports all of the Explorer Pro features. For analog
inputs, because they can be quite “chatty,” a differential threshold
mechanism was added to allow you to specify how much of a change in
input voltage must occur before publishing a value change notification.
You may specify a different value for each of the 4 analog inputs. Use
the "-t" command-line option to specify the threshold values. The
default values are 0.3 volts.

Also, because the Banyan Bot Blue does not require the use of analog
inputs, analog inputs are disabled by default. To turn them on, use the
"-a true" command-line option when starting the gateway.
[For testing,](https://github.com/MrYsLab/bots-in-pieces-examples/blob/master/banyan-bot-blue/tests/exp_pro_gateway/exp1.csv)
the test makes sure that analog inputs are enabled.

### Issues
There were 2 issues discovered using the Pimoroni library when
handling analog input. First, upon occasion, an erroneous value of 12
volts was returned. The gateway prevents this erroneous value from being
sent to the robot_control component by "throwing out" any value over 5.1
volts.

The second issue appears to be related to the library's threading
implementation for input values. Input values seemed to clobber one
other, so I added a [Python 
recursive lock](https://docs.python.org/3/library/threading.html#rlock-objects)
to protect all input handlers. The data clobbering problem was solved
although the analog inputs are still chatty.

In testing, for some unknown reason, coverage indicated that the digital 
input code did not execute. Digital inputs do work correctly but required 
physical observation to verify.


# Effect On Other Banyan Components For The Project
Because the Explorer Pro gateway supports the OneGPIO specification, no
other components needed to be modified or replaced. They all work "out
of the box, supporting the "Write Once" philosophy.

# Controller Board Recommendation

For battery powered projects the winner is The Pimoroni Explorer Pro
HAT. In spite of its minor shortcomings it is a delight to use.

# Banyan Bot Blue Comes To A Close
The Banyan Bot Blue project was designed to illustrate 
the flexibility and reusable nature of Python Banyan components. 
I hope that you try Python Banyan and OneGPIO for your next project, 
and see how easy it is to create an application with a set of reusable Banyan components.
