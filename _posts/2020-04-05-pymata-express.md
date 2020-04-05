---
layout: post
title: "Pymata-Express Updated"
date:  2020-04-05 11:00:39 -0500
categories: pymata arduino
tags: gaming p2p
---

# Pymata-Express Version 1.11 Released!
An update of [Pymata-Express](https://mryslab.github.io/pymata-express/)
has been released. The API has been updated to be consistent with the
pymata4 API. The format for the data reported in callbacks has been updated
as follows:

| Callback | Prior To Version 1.11| Version 1.11 And Above |
|---------------------- |--------------|------|
| analog input	| [pin, current reported value, pin_mode, timestamp]| [pin_mode = 2, pin, current reported value, pin_mode, timestamp] 	|
| digital input |[pin, current reported value, pin_mode, timestamp]| [pin_mode = 0, pin, current reported value, pin_mode, timestamp]|	|
| hc-sr04| [pin, distance]	| [pin_mode=12, trigger pin number, distance, timestamp]|
| i2c | [Device address, data bytes] |[pin_mode=6, i2c device address, data bytes, timestamp]|

No other API changes were implemented. 

The [User's Guide](https://mryslab.github.io/pymata-express/) has been updated and improved.

Here is a list of its major features:

* Python 3.7 (or higher) compatible.
* It offers a simplified pure asyncio [API.](https://htmlpreview.github.com/?https://github.com/MrYsLab/pymata-express/blob/master/html/pymata_express/index.html)
* Uses asyncio concurrency techniques (threading) for high efficiency.
* It Is [FirmataExpress](https://github.com/MrYsLab/FirmataExpress) compatible (link speed runs at 115200)
* Data change events may be associated with a callback function for asynchronous 
notification, or polling may be used when a synchronous approach is desired.
* Each data change event is time-stamped and stored.
* A [full set of working examples](https://github.com/MrYsLab/pymata-express/tree/master/examples)
 are available for download online.




 

