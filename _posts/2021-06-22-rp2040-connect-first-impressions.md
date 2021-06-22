---
layout: post
comments: true
title: "First Impressions Of The Arduino Nano RP2040 Connect"
date:  2021-06-22-10:00:39 -0500
categories: Arduino RP2040
tags: MCU
---

![]({{site.url}}/images/nano_2040.jpg)


I just received my Arduino Connect RP2040 board, and I am sorry to say that getting this thing up and running has been somewhat of a nightmare. It kind of reminds me of using the now-discontinued Arduino Yun

Using the Arduino IDE 1.8.15, the quick-install instructions tell you to install the **Arduino Mbed OS Nano Boards** library using the boards' manager. Simple enough. I then selected the **Arduino Nano RP2040 Connect** from the boards' selection menu and set the correct COM port. Then I loaded the blink script and did a test compile. So far, all is well. However, when I tried to upload, I got a non-descript loading error message. I turned on all warnings and errors in the IDE, which did not offer any additional clues.

So, I turned to the Arduino forum to find a solution. I found several postings about first putting the board into program mode by pressing the on-board switch two times. I did so but did it help? Absolutely not. So I searched the forum again and found a posting that said that a post-install script needed to be run in Arduino IDE. This shell script is:

~/.arduino15/packages/arduino/hardware/mbed_rp2040/1.5.2/post_install.sh

So when I went to run this script, it was not there. It turns out you need to load yet another (unmentioned) library, that is, Arduino Mbed OS RP2040 Boards. Again, not mentioned in the documentation. Once this board's library is loaded, you can then run the post-install script. 

Finally, I could upload the blink example script. Lots of fun!

Now on to try out WiFi. Wish me luck!