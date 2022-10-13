---
layout: post
comments: true
title: "Banyan Bot Blue Part 2"
date:  2019-06-15 07:55:39 -0400
categories: Banyan-Bot-Blue Bluetooth Bluetooth-test-server
tags: pip pybluez
---

# Preparing The Raspberry Pi For Bluetooth Communication

# In this edition of *Bots In Pieces*, we will:
 
1. Configure the Raspberry Pi for Bluetooth RFCOMM communications.
2. Explain how to pair an Android device with the Raspberry Pi.
3. Install the [*pybluez*](https://github.com/pybluez/pybluez) Python package.
4. Install a Bluetooth Server test tool.

> **Don't have an Android device? Not to worry!**
>
> In a future post, a *tkinter* component will be presented that can be
used to replace the Android app and Bluetooth server. This will allow you
to drive the robot directly on the Raspberry Pi desktop.

## Configuring The Raspberry Pi, Bluetooth, and RFCOMM Communications.

As you will see, the process is pretty simple to do. Just follow the simple steps below. 

## Enabling The Raspberry Pi For Pairing.

### Step 1: Update Raspbian - open a terminal and type:
~~~ console
sudo apt-get update
sudo apt-get upgrade
~~~

### Step 2: In the terminal type the following:
```
sudo rfkill unblock all
```

This will unblock the Bluetooth device on your RPi if it is in a blocked state.

### Step 3: Invoke the *bluetoothctl* program:  

``` console
sudo bluetoothctl
```

Then enter the following three commands:

**power on**  
**discoverable on**  
**pairable on**  

You should see something like the following in your terminal:
```
pi@BanyanBot:~ $ sudo bluetoothctl
[NEW] Controller B8:27:EB:25:1A:EA BanyanBot [default]
[bluetooth]# power on
Changing power on succeeded
[bluetooth]# discoverable on
Changing discoverable on succeeded
[bluetooth]# pairable on
Changing pairable on succeeded
[bluetooth]# 

```

When first starting *bluetoothctl* the first line displays the RPi's Bluetooth MAC address
and hostname.

For each of the 3 commands, the tool will indicate success or failure.


## Pairing The Raspberry Pi To An Android Device
Consult your Android Device Manual for exact instructions. I am 
using a Kindle Fire Tablet as an example.

1. Power On your Android Device.

2. Go to the device's settings for Bluetooth and turn Bluetooth On.
![]({{site.baseurl}}/images/banyan-bot-blue-1/android_settings/android1.jpg)

3. Select "Pair A Bluetooth Device.

4. Perform a scan for Bluetooth devices and select the hostname of your Raspberry Pi when the scan completes.
![]({{site.baseurl}}/images/banyan-bot-blue-1/android_settings/android2.jpg)

5. You should see that the Android device is now paired with your Raspberry Pi
![]({{site.baseurl}}/images/banyan-bot-blue-1/android_settings/android3.jpg)

6. If you go back to the Raspberry Pi, and look at the output on bluetoothctl, you should
see something that looks like the following:
``` console
[bluetooth]# power on
Changing power on succeeded
[bluetooth]# discoverable on
Changing discoverable on succeeded
[CHG] Controller B8:27:EB:25:1A:EA Discoverable: yes
[bluetooth]# pairable on
Changing pairable on succeeded
[NEW] Device 00:BB:3A:06:A0:24 Alan's Kindle HDX
[CHG] Device 00:BB:3A:06:A0:24 Modalias: bluetooth:v001Dp1200d1436
[CHG] Device 00:BB:3A:06:A0:24 UUIDs: 0000110a-0000-1000-8000-00805f9b34fb
[CHG] Device 00:BB:3A:06:A0:24 UUIDs: 0000110c-0000-1000-8000-00805f9b34fb
[CHG] Device 00:BB:3A:06:A0:24 UUIDs: 00001200-0000-1000-8000-00805f9b34fb
[CHG] Device 00:BB:3A:06:A0:24 UUIDs: 00001800-0000-1000-8000-00805f9b34fb
[CHG] Device 00:BB:3A:06:A0:24 UUIDs: 00001801-0000-1000-8000-00805f9b34fb
[CHG] Device 00:BB:3A:06:A0:24 ServicesResolved: yes
[CHG] Device 00:BB:3A:06:A0:24 Paired: yes
[CHG] Device 00:BB:3A:06:A0:24 ServicesResolved: no
[CHG] Device 00:BB:3A:06:A0:24 Connected: no
```

It shows that pairing was successful. Notice that we are not connected yet.


## Enabling The Raspberry Pi For RFCOMM Serial Communication.

1. Close the bluetoothctl tool by either entering *exit* or pressing Control-D.

{:start="2"}
2. We are about to make some changes to the   
**/etc/systemd/system/dbus-org.bluez.service** file. 

Before doing so, let's make a back-up copy of the file in case anything goes wrong. In the terminal you have open
type:

```
sudo cp /etc/systemd/system/dbus-org.bluez.service /etc/systemd/system/dbus-org.bluez.service.BACKUP
```
This is a very long line so make sure that you copy it all.

{:start="3"}
3. Now we need to make changes to the file. We will modify one line and add a new line.
Open up the file with your favorite editor using sudo privileges:

``` console
sudo nano /etc/systemd/system/dbus-org.bluez.service
```

{:start="4"}
4. Here is what the file may look like:

``` console
[Unit]
Description=Bluetooth service
Documentation=man:bluetoothd(8)
ConditionPathIsDirectory=/sys/class/bluetooth

[Service]
Type=dbus
BusName=org.bluez
ExecStart=/usr/lib/bluetooth/bluetoothd
NotifyAccess=main
#WatchdogSec=10
#Restart=on-failure
CapabilityBoundingSet=CAP_NET_ADMIN CAP_NET_BIND_SERVICE
LimitNPROC=1
ProtectHome=true
ProtectSystem=full

[Install]
WantedBy=bluetooth.target
Alias=dbus-org.bluez.service
```

{:start="5"}

5. In the third line in the [Service] section, we need to add **-C** to the end of the line. Change:

``` console
ExecStart=/usr/lib/bluetooth/bluetoothd
```
to:

```
ExecStart=/usr/lib/bluetooth/bluetoothd -C
```

{:start="6"}

6. And right below that line, we need to add the following new line:
``` console
ExecStartPost=/usr/bin/sdptool add SP
```

The [Service] section should now look like:

``` console
[Service]
Type=dbus
BusName=org.bluez
ExecStart=/usr/lib/bluetooth/bluetoothd -C
ExecStartPost=/usr/bin/sdptool add SP
NotifyAccess=main
#WatchdogSec=10
#Restart=on-failure
CapabilityBoundingSet=CAP_NET_ADMIN CAP_NET_BIND_SERVICE
LimitNPROC=1
ProtectHome=true
ProtectSystem=full
```

{:start="7"}

7. Save the file and exit the editor.
8. Reboot the Raspberry Pi.

# Installing A Bluetooth Test Server

In the next post, we will be connecting an Android app to our Raspberry Pi using
Bluetooth. This app requires a Bluetooth server to be running on the RPi, and so
let's install one now.

## First, Let's Update *pip* To The Latest And Greatest.

The server requires the [*pybluez*](https://github.com/pybluez/pybluez) Python package to
be installed on the Raspberry Pi. Before doing so, let's take the opportunity to update
*pip* on the RPi to the latest version. If you are running Raspbian Buster you can skip this step.

Open a terminal window on your Raspberry Pi and type the following command:
```console
sudo pip3 install -U pip
```
## Install The *pybluez* Library

First install the required libraries for pybluez.
In your console type:
```bash
sudo apt-get install libbluetooth-dev
```

Next, in your console type:
```
sudo pip3 install pybluez
```

## Copy The Test Server To Your Raspberry Pi
The source code for the server [*may be found at this link*.](https://github.com/MrYsLab/bots-in-pieces-examples/blob/master/banyan-bot-blue/test_fixtures/bluetooth_apk_validation_server.py)
Create a *banyan-bot-blue* directory on your Raspberry Pi, then create a *test_fixtures* directory. Copy the test
server to that directory.

```console
banyan-bot-blue
└── test_fixtures
    └── bluetooth_apk_validation_server.py
```

If you look at the server code you will see that the server initially prints out a
message that it is waiting for a client to connect.
```python
import bluetooth


print('Waiting for client to connect...')
server_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)

port = 1
server_sock.bind(("", port))
server_sock.listen(1)
```

 
 After a successful connection, it sends
a short text message to the client and then sits in a loop receiving data from the
client and printing the data to the console.

```python
client_sock, address = server_sock.accept()

print("Accepted connection from ", address)
data = 'Hello Banyan Bot'
data = data.encode()
client_sock.send(data)

while True:
    data = (client_sock.recv(1024)).decode()
print("received [%s]" % data)
```
    
To run the server it must be invoked with sudo:
```console
sudo python3 bluetooth_apk_validation_server.py

```
You should see the *Waiting for client to connect...* message.
You can then kill the server.
 
### That concludes this post. Next time - we build an Android app to control the robot and connect it to the server.




