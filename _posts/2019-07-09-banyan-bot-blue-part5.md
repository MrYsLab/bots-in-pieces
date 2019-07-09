---
layout: post
title: "Banyan Bot Blue Part 5"
date:  2019-07-09 19:24:39 -0400
categories: banyan-components bluetooth-gateway
tags: threading banyan coding-convention
---


![]({{site.url}}/images/banyan-bot-blue-1/car.jpg)
{: style="float:left; "}
# The Bluetooth Gateway   
{: style="color:blue;  "}
# *A Peek Under The Hood*   
{: style="color:blue;  "}
   
<br /> 
<br />

# In this edition of *Bots In Pieces*, we will:
* Discuss the Banyan Bluetooth Gateway (**BTG**) component:
  * Its purpose.
  * A peek at the code:
    * What's inside.
    * Adding threading.Thread to a Banyan component.
    * Transforming a Banyan component into a command-line executable.

# What Is The Bluetooth Gateway?

The BTG is a Banyan *Protocol Gateway* that supports seamless bidirectional
data transfer between a Bluetooth device and the Banyan network. It re-formats Bluetooth data received from a Bluetooth
device into Banyan protocol messages, 
while simultaneously receiving Banyan messages and reformatting that data to a format
 that can be sent over the RFCOMM link to the Bluetooth device. 

By default, the BTG acts as
a Bluetooth RFCOMM *server.* 


  ![]({{site.url}}/images/banyan-bot-blue-1/bluetooth_gateway.png)
  

The Bluetooth Gateway was designed to be as general-purpose as possible in order to support
a variety of application scenarios without the need to recode the component. 
It accomplishes this goal by offering a set of command-line 
options available to the user at launch time to affect the BTG's behavior.

## The Bluetooth Gateway Command-Line Options

Here is a list of all the command-line options
 that the BTG supports:

```shell
          -a SERVER_BT_ADDRESS  Bluetooth MAC Address of Bluetooth Gateway
          -b BACK_PLANE_IP_ADDRESS
                                None or IP address used by Back Plane
          -g GATEWAY_TYPE       Type of Gateway: server or client
          -j JSON_DATA          Bluetooth packets json encoded True or False
          -l PUBLISH_TOPIC      Banyan publisher topic
          -m SUBSCRIBER_LIST [SUBSCRIBER_LIST ...]
                                Banyan topics space delimited: topic1 topic2 topic3
          -n PROCESS_NAME       Set process name in banner
          -p PUBLISHER_PORT     Publisher IP port
          -s SUBSCRIBER_PORT    Subscriber IP port
          -t LOOP_TIME          Event Loop Timer in seconds
          -u UUID               Bluetooth UUID

```

* SERVER_BT_ADDRESS - if you configure the gateway as an RFCOMM *client*, 
you will need to specify its Bluetooth MAC address. By default, the BTG is configured
as a Bluetooth *server,* and for server mode, the address does not need to be specified.
The default value for this option is *None.*

* BACK_PLANE_IP_ADDRESS - this is a common option for all Banyan components. Banyan allows you to 
[*distribute components across multiple computers.*](https://mryslab.github.io/python_banyan/#example3/)
This option allows you to explicitly specify the IP address of the computer that is
hosting the backplane. The default is *None,* which automatically sets
 the backplane IP address to the IP address of the local computer. 

* GATEWAY_TYPE - select either server or client. Default is server.

* JSON_DATA - The BTG can be configured to treat data over the Bluetooth RFCOMM
link
as either JSON or string data. Setting this option to True enables
JSON encoding, and False enables string encoding. The 
default option value is *False* for string encoding. 

* PUBLISH_TOPIC - the topic string used when the BTG publishes Banyan messages
containing Bluetooth data. The default is *from_bt_gateway.*

* SUBSCRIBER_LIST - a space-delimited list of BTG subscription topics.
 Default is a single topic of
*to_bt_gateway.*

* PROCESS_NAME - The name of the component that will be displayed in the console header
when the gateway is launched. Default is
*BanyanBluetoothServer.* If you've selected a GATEWAY_TYPE of *client*, the process_name 
will be *BanyanBluetoothClient.*
 
* PUBLISHER_PORT -  This option specifies the IP port used when publishing Banyan messages. 
The default is *43124.* You may choose a different port number if this
port is already in use, 
 or if you have configured the Banyan network as a 
[*multi-backplane network.*](https://mryslab.github.io/python_banyan/#example9/)

* SUBSCRIBER_PORT - This option specifies the IP port used for receiving Banyan messages. 
The default is *43125.* You may choose a different port number if this
port is already in use, 
 or if you have configured the Banyan network as a 
[*multi-backplane network.*](https://mryslab.github.io/python_banyan/#example9/)

* LOOP_TIME - the time period that the Banyan receive loop remains idle before 
checking to see if the next received message
is available for processing. This option allows you to tune the component for 
performance and 
CPU utilization. The default value is *0.01 seconds.*

* UUID - A Unique Bluetooth service identifier. 
Default is:   
*e35d6386-1802-414f-b2b9-375c92fa23e0.*  



## The Code

In this section, we will be examining selected portions of the code.
You can view the code in its entirety
[*here.*](https://github.com/MrYsLab/bots-in-pieces-examples/blob/master/banyan-bot-blue/banyan_assets/bluetooth_gateway.py)

> If you have any questions about the code, please leave a comment at the bottom of this page.

As explained in a 
[*previous posting,*](http://0.0.0.0:4000/banyan-framework/2019/06/26/banyan-background-4.html)
 in general, to create a Banyan component the programmer:

* Creates a class that inherits from the Python Banyan base class.
* Overwrites the \__init__ method.
* Calls the parent class \__init__ method.
* Overwrites the incoming_message_processing method to handle Banyan messages.

## Discussing The Code
For code discussions, first, a section of the source will be shown, 
followed by a discussion of that section.


### The Imports

```python
import argparse
import json
import sys
import subprocess
import threading

from bluetooth import *

from boltons.socketutils import BufferedSocket

from python_banyan.banyan_base import BanyanBase

```

At the top of the file, as is typical, all of the modules required by the Bluetooth Gateway are imported.

If you followed the previous postings, any libraries required to be installed have
 already be installed. 

> **Note**: When importing the Banyan base class, use the syntax shown above.




### Declaring The BluetoothGateway Class

```python
class BlueToothGateway(BanyanBase, threading.Thread):
```
All Banyan components inherit from the *BanyanBase* base class.
In order to support bidirectional
data transmission over the RFCOMM link, the BTG also needs to inherit from
*threading.Thread.* A separate thread handles Bluetooth data reception. Thread creation and execution is accomplished in the
standard manner.

### The \__init__ Method

```python
   def __init__(self, back_plane_ip_address=None, subscriber_port='43125',
                 publisher_port='43124', process_name=None, loop_time=.001,
                 gateway_type=BTG_SERVER, publish_topic=None,
                 uuid='e35d6386-1802-414f-b2b9-375c92fa23e0',
                 server_bt_address=None, subscriber_list=None,
                 json_data=False):
 ```
 Here we see all of the parameters that the class accepts. We could have used 
 **kwargs here, but I think it is clearer to list everything out. All parameters are
 *key-word* parameters, so all have a default value.
 
 
 ```python
        """
        This method initializes the class for operation
        """
        # save input parameters as instance attributes
        self.back_plane_ip_address = back_plane_ip_address
        self.subscriber_port = subscriber_port
        self.publisher_port = publisher_port
        self.loop_time = loop_time
        self.gateway_type = gateway_type

        # set the name for the banner depending upon client or server
        if process_name is None:
            if self.gateway_type == self.BTG_CLIENT:
                self.process_name = 'BanyanBluetoothClient'
            else:
                self.process_name = 'BanyanBluetoothServer'
        else:
            self.process_name = process_name

        self.publish_topic = publish_topic

        self.uuid = uuid
        self.server_bt_address = server_bt_address
        self.json_data = json_data
        
```

In the section above we save the input parameters as instance attributes so that they
are available to be used anywhere within the class.

One of the parameters, *gateway_type* is used to define the process name that will 
be displayed when the Bluetooth Gateway is invoked.

```python

        # initialize the parent

        super(BlueToothGateway, self).__init__(
            back_plane_ip_address=self.back_plane_ip_address,
            subscriber_port=self.subscriber_port,
            publisher_port=self.publisher_port,
            process_name=self.process_name,
            loop_time=self.loop_time)

```
Above we call *super* to initialize the BanyanBase parent.

```python
        self.subscriber_list = subscriber_list
        
                for topic in self.subscriber_list:
                    self.set_subscriber_topic(topic)
                    print('Subscribed to: ', topic)
        
        print('Publish to   : ', self.publish_topic)
```
In the code above we subscribe to the topics specified by calling *set_subscriber_topic* 
for each topic,
and printing
each *subscription topic* to the console. The specified *publishing topic* is also printed to the console.

```python
        mac = self.find_local_mac_address()
        if mac:
            print('Local Bluetooth MAC Address: ', mac)
        else:
            print('No Bluetooth Interface Found - Exiting')
            sys.exit(0)
```

Here we self-discover the MAC address of the local Bluetooth interface and print it to the console.

The [*find_local_mac_address*](https://github.com/MrYsLab/bots-in-pieces-examples/blob/04b6adace07f3449bfacc6dd3f045c5bd8199ec0/banyan-bot-blue/banyan_assets/bluetooth_gateway.py#L2020)
 method utilizes *subprocess.Popen* to execute an *hcitool* command to 
retrieve the MAC address.

```python
        if self.gateway_type == self.BTG_SERVER:
            self.server_sock = BluetoothSocket(RFCOMM)
            self.server_sock.bind(("", PORT_ANY))
            self.server_sock.listen(1)

            port = self.server_sock.getsockname()[1]

            advertise_service(self.server_sock, "BanyanBlueToothServer",
                              service_id=uuid,
                              service_classes=[uuid, SERIAL_PORT_CLASS],
                              profiles=[SERIAL_PORT_PROFILE],
                              )

            print("Waiting for connection on RFCOMM channel %d" % port)

            self.client_sock, self.client_info = self.server_sock.accept()

            print("Accepted connection from ", self.client_info)
```

In the section of code above, we create a BluetoothServer and then wait for an incoming
connection.

```python
        else:
            service_matches = find_service(uuid=self.uuid,
                                           address=self.server_bt_address)

            if len(service_matches) == 0:
                print("couldn't find the SampleServer service =(")
                sys.exit(0)

            first_match = service_matches[0]
            port = first_match["port"]
            name = first_match["name"]
            host = first_match["host"]

            print("connecting to \"%s\" on %s" % (name, host))

            # Create the client socket
            self.client_sock = BluetoothSocket(RFCOMM)
            self.client_sock.connect((host, port))

```

Above, if the Bluetooth Gateway was configured to be a client, the *else* path is taken.

```python
        # wrap the socket for both client and server
        self.bsock = BufferedSocket(self.client_sock)

        # create a thread to handle receipt of bluetooth data
        threading.Thread.__init__(self)
        self.daemon = True

        # start the thread
        self.start()

        # this will keep the program running forever
        self.receive_loop()
```

And in the last section of \__init__ shown above, we wrap the socket with a 
[*Bolton BufferedSocket.*](https://boltons.readthedocs.io/en/latest/socketutils.html#bufferedsocket)

We do this to simplify parsing JSON data if the BTG was configured for JSON
encoding/decoding.

We then initialize the parent Thread class and then start the thread to 
receive data from the Bluetooth connected device.  

Next, we start
the banyan *receive_loop* that keeps the program running while processing
any incoming Banyan protocol messages.


### The incoming_message_processing Method

```python
    def incoming_message_processing(self, topic, payload):
        """
        Process the incoming Banyan message to
        be sent to the Bluetooth network
        :param topic: topic string
        :param payload: payload data
        """

        # if the bluetooth device requires json encoding
        if self.json_data:
            data_out = json.dumps(payload)
            data_out = data_out.encode('utf-8')

            try:
                self.bsock.send(data_out)
            except Exception as e:
                self.clean_up()
                raise RuntimeError('Write Error')
        else:
            # convert the payload to a string
            data_out = str(payload['report'])
            data_out = data_out.encode('utf-8')
            self.client_sock.send(data_out)
```
The code above is the entirety of this method. It is automatically called 
by the base class when an incoming Banyan protocol message
is received. If the hardware interface component detects a status change on a GPIO
pin, it may generate a Banyan message containing that change. The BTG can register
to receive status change messages and pass the change data to the Bluetooth device.

The method checks to see if the JSON data option is enabled and if so, the data is JSON encoded
and sent across the RFCOMM link.

If JSON is not enabled, the *report* value is extracted, converted to a string, 
 encoded
and sent across the RFCOMM link.

### The run Method

```python
    def run(self):
        """
        This is thread that receives packets from the bluetooth interface
        :return:
        """

        while True:
            # if json encoding look for termination character
            # used for a dictionary
            if self.json_data:
                try:
                    data = self.bsock.recv_until(b'}',
                                                 timeout=0,
                                                 with_delimiter=True)
                except Exception as e:
                    continue

                data = data.decode()
                data = json.loads(data)

                self.publish_payload(data, self.publish_topic)

            # data is not json encoded
            else:
                data = (self.client_sock.recv(1)).decode()
                payload = {'command': data}
                self.publish_payload(payload, self.publish_topic)

```
Bluetooth data is received and processed by the thread implementation shown above.
The data is either treated as JSON data or as string data depending upon the JSON
command line option selected. 
 In either case, a Banyan protocol message is created containing
the data and then published to the Banyan network.

### Capturing User Specified Command-Line Options And Starting The Component

```python
def bluetooth_gateway():
    parser = argparse.ArgumentParser()
    parser.add_argument("-a", dest="server_bt_address", default="None",
                        help="Bluetooth MAC Address of Bluetooth Gateway"),
    parser.add_argument("-b", dest="back_plane_ip_address", default="None",
                        help="None or IP address used by Back Plane")
    parser.add_argument("-g", dest="gateway_type", default="server",
                        help="Type of Gateway : server or client"),
    parser.add_argument("-j", dest="json_data", default="False",
                        help="Bluetooth packets json encoded true or false"),
    parser.add_argument("-l", dest="publish_topic", default="from_bt_gateway",
                        help="Banyan publisher topic"),
    parser.add_argument("-m", dest="subscriber_list",
                        default=["None"], nargs="+",
                        help="Banyan topics space delimited: topic1 topic2 "
                             "topic3")
    parser.add_argument("-n", dest="process_name", default="None",
                        help="Set process name in banner")
    parser.add_argument("-p", dest="publisher_port", default='43124',
                        help="Publisher IP port")
    parser.add_argument("-s", dest="subscriber_port", default='43125',
                        help="Subscriber IP port")
    parser.add_argument("-t", dest="loop_time", default=".01",
                        help="Event Loop Timer in seconds")
    parser.add_argument("-u", dest="uuid",
                        default="e35d6386-1802-414f-b2b9-375c92fa23e0",
                        help="Bluetooth UUID")

    args = parser.parse_args()

    if args.back_plane_ip_address == 'None':
        args.back_plane_ip_address = None
    if args.server_bt_address == 'None':
        args.backplane_ip_address = None
    if args.gateway_type == 'server':
        args.gateway_type = BlueToothGateway.BTG_SERVER
    else:
        args.gateway_type = BlueToothGateway.BTG_CLIENT
    if args.server_bt_address == 'None':
        args.server_bt_address = None
    if args.process_name == 'None':
        args.process_name = None
    if args.subscriber_list == ['None']:
        args.subscriber_list = ['to_bt_gateway']
    if args.json_data == 'False' or args.json_data == 'false':
        args.json_data = False
    else:
        args.json_data = True

    kw_options = {
        'back_plane_ip_address': args.back_plane_ip_address,
        'publisher_port': args.publisher_port,
        'subscriber_port': args.subscriber_port,
        'process_name': args.process_name,
        'json_data': args.json_data,
        'loop_time': float(args.loop_time),
        'publish_topic': args.publish_topic,
        'gateway_type': args.gateway_type,
        'uuid': args.uuid,
        'server_bt_address': args.server_bt_address,
        'subscriber_list': args.subscriber_list
    }


    try:
        app = BlueToothGateway(**kw_options)
    except KeyboardInterrupt:
        sys.exit()

    # noinspection PyUnusedLocal
    def signal_handler(sig, frame):
        print("Control-C detected. See you soon.")
        app.clean_up()
        sys.exit(0)

    # listen for SIGINT
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)


if __name__ == '__main__':
    bluetooth_gateway()
```

Python Banyan Components typically allow a user to *tune* the component by specifying
command-line options when invoking the component. This is accomplished within the
*blue_tooth_gateway* function, using the 
[*argparse*](https://docs.python.org/3.7/howto/argparse.html)
module. 

In addition to handling command-line options, the BluetoothGateway class is also 
instantiated
within this function.

Notice that this function and everything below it is outside of the definition of the 
BluetoothGateway class. 

Below the *blue_tooth_gateway* function, there is a signal handler to trap Control-C presses.

And the very last line calls the bluetooth_gateway function to instantiate the gateway.

> **Installing a component as a command-line executable:**
The code below the BluetoothGateway class definition is written using a specific
 stylized structure and is common in many of the Banyan components. This allows one to 
 easily
  [*transform a component 
into a command-line executable.*](https://mryslab.github.io/python_banyan/#example7/)

Keeping in line with the Banyan design philosophy, the BluetoothGateway is a component
that is very limited in scope. In total it has less than 150 executable statements.
As a result, testing is simplified, and the possibility of reuse increased.
As we look at other examples of Banyan components in future posts, we will see
a lot of similarity in file structure. Using common Banyan coding patterns when 
developing
Banyan components aids in the rapid development of quality code.

## Next Time

Next time, we will test the Bluetooth Gateway and learn about BlackBox and WhiteBox
testing. We will learn how to use the Banyan *monitor* to view Banyan messages
as they are published, and how to use the Banyan launcher to launch several
components and tools from a single command line, making testing simple and
reproducible.
