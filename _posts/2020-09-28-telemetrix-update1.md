---
layout: post
comments: true
title: "Telemetrix Update 1"
date:  2020-09-27 11:00:39 -0500
categories: arduino,stm32,firmata
tags: arduino
---
I intended this posting to be  a discussion of both the client and server code,
but I am postponing that discussion until the next posting when phase 2 is complete.

I am converting the server sketch into a library
so that it can be installed from the Arduino Library Manager.

I am refactoring the sketch to make it simpler to add new commands.
Also, I will be renaming the library to Telemetrix4Arduino. There may be an issue with
the Arduino trademark guidance with the previous name. These changes do not alter the phase 1 functionality, and
the phase 1 client works with the updated server.

I am also integrating the Servo library.
 By doing so, I will better understand organizing the Arduino code so 
 that users do not have to go hunting around for the items that need modification when adding new commands or reports.
The intent is to have everything that the user needs at the top of the file, clearly
commented.

As a result, I can concentrate the next discussion on the code one would need to augment to add features,
while providing some background on how the code works.



