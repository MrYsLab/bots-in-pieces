---
layout: post
comments: true
title: "Phase 2 - Server And Client Details"
date:  2020-10-03 10:00:39 -0500
categories: arduino,telemetrix,servo
tags: arduino,servo
---

In this posting, we will discuss the phase 2 release.

This release incorporates support for servos and refactors some of the Arduino code.
Here is what will be covered today:
* Installation of the phase2 server and client code.
* A short discussion of what was changed in phase 2.
* A quick look at possible future changes.
* A discussion of the steps used in adding the servo support. This discussion
will include a partially detailed code description, and when appropriate, a "discussion detour" to
explain some concepts not directly part of the topic at hand.


Support for servos is now available in the 
[phase 2 directory of the telemetrix repository.](https://github.com/MrYsLab/telemetrix/tree/master/phase2)

An API is available at [this link.](https://htmlpreview.github.com/?https://github.com/MrYsLab/telemetrix/blob/master/phase2/api/index.html)

There have been some significant changes made to the phase 1 code, and these will be discussed in a moment.

For those who wish to try things out, here is what you need to do to install phase 2.

1. Go to the [telemetrix repository](https://github.com/MrYsLab/telemetrix) and click on the green 
code button to download the .zip file.

2. Extract the files into a directory of your choice. A directory, called telemetrix-master, will be created, and
it will contain all the necessary files.

3. Install the latest Arduino code by opening the Arduino IDE. From the main menu, go to *Sketch*, 
select *Include Library*, and then select *Add .ZIP Library*. Select the Telemetrix4Arduino.zip file in the 
telemetrix-master/phase2 directory. Finally, click on *File* from the IDE main menu and select *Examples* and then
select Telemetrix4Arduino and upload the code to your device.

4. To install the phase2 Python code, cd to telemetrix-master/phase2, and then type:

```
# For Windows:

python setup.py install


# Or for macOS and Linux:


sudo python3 setup.py install
```
You are now ready to try out the examples in the telemtrix-master/examples directory using phase 2 code. 
Note that you may need to change pin numbers in the examples to match your
hardware setup. All of the examples require the use of Python 3.

# What Has Changed In Phase 2

* The Arduino code has been placed in an Arduino Library directory structure.
   * At some future time, the Arduino Library Manager may be used to install the code into the IDE. 
   Currently, you need to install using
the .zip file, as shown above, since this library has yet to be published to be included in the Library Manager. All the code 
is maintained in a single file to help simplify adding features.
* The Arduino code was renamed Telemetrix4Arduino.
   * The renaming was done to assure that there is no violation of the Arduino trademark.
* A bug was fixed in processing incoming commands. The command handler was being called multiple times, and now, correctly,
it is called once.
*  Arduino code was reorganized to make it easier to expand and add functionality.

# What Might Change For A Future Release
Besides adding new features, I am considering using two additional Arduino libraries
to make the code easier to understand and maintain. 

The first library is [ArduinoJson.](https://arduinojson.org/) Currently, 
both command and report data is treated as an array of bytes. Indices into the array are used to create
and parse these arrays. By using ArduinoJson, the data will be able to parsed using names instead of indices.
Also, I intend to use the MessagePack feature of ArduinoJson to minimize the size of the messages being sent across
the serial link.

The second library is the [Embedded Template Library(ETL).](https://www.etlcpp.com/) 
ETL will allow the creation of more cohesive 
data structures to make the code easier to understand and maintain.

When evaluating these libraries, the gating factors are the amount of memory space they will use 
and the impact on the speed of data transfers
 because the number of bytes for a given command or report will be somewhat larger than using the current array of bytes scheme.

If these libraries are viable for Telemetrix4Arduino, it is not certain as of yet in which phase
they will be included. My current thinking is for phase 4 after I have implemented i2c support.

# The Steps Used To Add Servo Support To The Arduino Code
Instead of going through all the code line by line, it is more beneficial to describe the steps
used in adding servo support. The code involved in each step will be both shown and discussed. 
If you are going through the code in detail and have any questions or comments, please post them below.

## Step 1: Decide Which Library Functions You Wish To Support
I decided to expose the _attach_, _write_, and _detach_ methods for client access via command messages for the 
[servo library](https://www.arduino.cc/reference/en/libraries/servo/). Internally, the _attached_ method is used
but not exposed directly to the client.

## Step 2: Decide On Method Names To Support The New Commands And Expose Them As _externs_
By defining the command functions using extern, the compiler will support forward referencing, and we
don't need to worry about where the actual commands are defined within the file.
```
// We first define the command functions here to provide a forward reference.
// If you add a new command, you must add the command handler
// here as well.

// Phase 1 commands
extern void serial_loopback();

extern void set_pin_mode();

extern void digital_write();

extern void analog_write();

extern void modify_reporting();

extern void get_firmware_version();

extern void are_you_there();

// Phase 2 commands - supporting the servo library
extern void servo_attach();

extern void servo_write();

extern void servo_detach();
```
## Step 3: Add #defines For The New Command Values And Reports
```
// Commands -received by this sketch
// Add commands retaining the sequential numbering.
// The order of commands here must be maintained in the command_table.
#define SERIAL_LOOP_BACK 0
#define SET_PIN_MODE 1
#define DIGITAL_WRITE 2
#define ANALOG_WRITE 3
#define MODIFY_REPORTING 4 // mode(all, analog, or digital), pin, enable or disable
#define GET_FIRMWARE_VERSION 5
#define ARE_U_THERE  6
#define SERVO_ATTACH 7
#define SERVO_WRITE 8
#define SERVO_DETACH 9

// Reports - sent from this sketch
#define DIGITAL_REPORT DIGITAL_WRITE
#define ANALOG_REPORT ANALOG_WRITE
#define FIRMWARE_REPORT 5
#define I_AM_HERE 6
#define SERVO_UNAVAILABLE 7
#define DEBUG_PRINT 99
```
The SERVO_UNAVAILABLE report is sent to the client if a servo_attach fails because
there are no available servos.

## Step 4: Add Entries Into The command_table For The New Commands
```
// When adding a new command update the command_table.
// The command length is the number of bytes that follow
// the command byte itself, and does not include the command
// byte in its length.
// The command_func is a pointer the command's function.
typedef struct command_descriptor {
    byte command_length;
    void (*command_func)(void);
};
command_descriptor command_table[10] =

        {
                {1, &serial_loopback},
                {4, &set_pin_mode},
                {2, &digital_write},
                {2, &analog_write},
                {1, &modify_reporting},
                {0, &get_firmware_version},
                {0, &are_you_there},
                {5, &servo_attach},
                {2, &servo_write},
                {1, &servo_detach},
        };
```
The command_table is an array of command_descriptors. The command value is used as an index into the array,
so the command position in the table must match the command values defined in step 3.

Make sure to increase the size of the table with each added command.

A command_descriptor consists of two values. The first is the number of bytes of
data that need to be retrieved after the command byte is received.
 The second is the address of the function that performs the processing of the command.

**DETOUR**: The command function addresses are stored in the _command table_ so that we can 
avoid having to use a large switch statement or a long list of if/else
statements to de-reference a command.
Instead, _find_next_command_ uses the C language function pointer
syntax to call the command function using the address in the table.

```
void get_next_command() {
    byte command;
    int command_buffer_index = 0;
    command_descriptor command_entry;

    for (int i = 0; i < MAX_COMMAND_LENGTH; i++) {
        command_buffer[i] = 0;
    }
    if (Serial.available()) {
        // get the command byte
        command = (byte) Serial.read();
        // uncomment the next line to see the command byte value
        //send_debug_info(75, command);
        command_entry = command_table[command];
        //send_debug_info(command, command_entry.command_length);
        // get the data for that command
        if (command_entry.command_length > 0) {
            for (int i = 0; i < command_entry.command_length; i++) {
                // need this delay or data read is not correct
                delay(1);
                if (Serial.available()) {
                    command_buffer[command_buffer_index++] = (byte) Serial.read();
                    // uncomment out to see each of the bytes followning the command
                    //send_debug_info(3, (int) command_buffer[command_buffer_index - 1]);
                }
            }
        }
        (command_entry.command_func());
    }

}
```

## Step 5: Allocate Any Global Storage That The Library Requires
There is a fixed maximum number of servos that the library supports. 
Here we create an array of Servo objects. The servo library defines the MAX_SERVOS value.

```
// servo management
Servo servos[MAX_SERVOS];
```
When a _servo attach_ command is received, we need to determine if there 
are any servo objects available for use. To do this, we create
an array of bytes that will aid in finding the first available servo object
that we may use. If an available servo is found, then
the pin number is entered in the pin_to_servo_index_map array, using the index returned by  the
_find_servo_ function described below.
```

byte pin_to_servo_index_map[MAX_SERVOS];

```
## Step 6: Implement The Command Functions
```
/***************************************************
 * Servo Commands
 **************************************************/

// Find the first servo that is not attached to a pin
int find_servo() {
    int index = -1;
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (servos[i].attached() == false) {
            index = i;
            break;
        }
    }
    return index;
}
```
The _find_servo_ function is a helper function that iterates through the servos array
to find the first available servo for use. It sequentially checks to see if a servo 
is currently attached. If not, it returns the index in the servo array. If there are
no servos available, it returns a -1.
```
void servo_attach() {

    byte pin = command_buffer[0];
    int servo_found = -1;

    int minpulse = (command_buffer[1] << 8) + command_buffer[2];
    int maxpulse = (command_buffer[3] << 8) + command_buffer[4];

    // find the first avalable open servo
    servo_found = find_servo();
    if (servo_found != -1) {
        pin_to_servo_index_map[servo_found] = pin;
        servos[servo_found].attach(pin, minpulse, maxpulse);
    } else {
        // no open servos available, send a report back to client
        byte report_message[2] = {SERVO_UNAVAILABLE, pin};
        Serial.write(report_message, 2);
    }
}
```
The _servo_attach_ function is called by the _get_next_command_ function when de-referencing an incoming
_SERVO_ATTACH_ command.
 
Five additional bytes, in addition to the command byte, are required to process the _servo_attach_ command.
The first byte is the pin number. The next 2 bytes contain the high and low-value bytes
for the minimum pulse width to be used. These bytes are combined to form an integer.
The last 2 bytes contain the high and low-value bytes for the maximum pulse width. These
2 bytes are also combined to form an integer value.

The _servo_attach_ function calls _find_servo_. If a servo is available, the pin
number is associated with the servo in the _pin_to_servo_index_map_ array.
If there are no available servos, a SERVO_UNAVAILABLE report is formed and sent across the serial link. 

**DETOUR**: In order to send values that are larger than a byte (ints, floats, etc.), the larger value needs 
to be broken down into byte-size chunks. By convention, we send the high order byte first, followed by the low order bytes.
 The receiver then reconstructs the actual value by combining the individual bytes in their proper order
to re-form the original value. Hopefully, ArduinoJson will preclude having to assemble and disassemble data
values greater than a byte in length.
```

// set a servo to a given angle
void servo_write() {
    byte pin = command_buffer[0];
    int angle = command_buffer[1];
    servos[0].write(angle);
    // find the servo object for the pin
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (pin_to_servo_index_map[i] == pin) {
            servos[i].write(angle);
            return;
        }
    }
}
```
The _servo_write_ command associates the supplied pin number with its associated 
servo instance, and then writes the supplied angle to the servo.
```

// detach a servo and make it available for future use
void servo_detach() {
    byte pin = command_buffer[0];

    // find the servo object for the pin
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (pin_to_servo_index_map[i] == pin) {

            pin_to_servo_index_map[i] = -1;
            servos[i].detach();
        }
    }
}
```
The _servo_detach_ command disassociates the servo instance from the pin and makes
the servo instance available for reuse.

# The Steps Used To Add Servo Support To The Python Client Code

## Step 1: Update _private_constants.py_
First, add the new commands and reports to private_constants.py. These values must match
their counterparts defined in the server.

```
# commands
# send a loop back request - for debugging communications
LOOP_COMMAND = 0
SET_PIN_MODE = 1  # set a pin to INPUT/OUTPUT/PWM/etc
DIGITAL_WRITE = 2  # set a single digital pin value instead of entire port
ANALOG_WRITE = 3
MODIFY_REPORTING = 4
GET_FIRMWARE_VERSION = 5
ARE_U_THERE = 6  # Arduino ID query for auto-detect of telemetrix connected boards
SERVO_ATTACH = 7
SERVO_WRITE = 8
SERVO_DETACH = 9

# reports
# debug data from Arduino
DIGITAL_REPORT = DIGITAL_WRITE
ANALOG_REPORT = ANALOG_WRITE
FIRMWARE_REPORT = GET_FIRMWARE_VERSION
I_AM_HERE_REPORT = ARE_U_THERE
SERVO_UNAVAILABLE = SERVO_ATTACH
DEBUG_PRINT = 99
```
**DETOUR**: The ARE_U_THERE command and I_AM_HERE report are used to allow the client
to perform autodiscovery of a specific board plugged into your computer. The value
returned by the I_AM_HERE report must match the expected Arduino ID value.
You may change the Arduino ID values to accommodate multiple boards by setting the ID 
in the Telemetrix4Arduino sketch and using a matching value when instantiating the Telemetrix 
class. 

## Step 2: For New Reports Update The report_dispatch Dictionary
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
```
By adding the name of the method to this dictionary, the callback for the report is invoked
in a similar fashion to using a function pointer in C++.

## Step 3: Implement The Callback Method For The Report
```
    def _servo_unavailable(self, report):
        """
        Message if no servos are available for use.
        :param report: pin number
        """
        raise RuntimeError(f'Servo Attach For Pin {report[0]} Failed: No Available Servos')
```
A RuntimeError is raised if no servos were available.

## Step 4: Implement The Command Methods 

### set_pin_mode_servo

The _set_pin_mode_servo_ method is used to request a pin to be
attached to a servo.
```
    def set_pin_mode_servo(self, pin_number, min_pulse=544, max_pulse=2400):
        """
        Attach a pin to a servo motor
        :param pin_number: pin
        :param min_pulse: minimum pulse width
        :param max_pulse: maximum pulse width
        """
        minv = (min_pulse).to_bytes(2, byteorder="big")
        maxv = (max_pulse).to_bytes(2, byteorder="big")

        command = [PrivateConstants.SERVO_ATTACH, pin_number,
                   minv[0], minv[1], maxv[0], maxv[1]]
        self._send_command(command)
```

### servo_write

The servo_write method is used to move the servo to an angle between 0 and 
180 degrees.

```
   def servo_write(self, pin_number, angle):
        """
        Set a servo attached to a pin to a given angle
        :param pin_number: pin
        :param angle: angle (0-180)
        """
        command = [PrivateConstants.SERVO_WRITE, pin_number, angle]
        self._send_command(command)
```

### servo_detach

To free-up a servo for reuse, servo_detach is called.

```
    def servo_detach(self, pin_number):
        """
        Detach a servo for reuse
        :param pin_number: attached pin
        """
        command = [PrivateConstants.SERVO_DETACH, pin_number]
        self._send_command(command)
```

# Concluding Remarks
Adding support for a new library is accomplished by following the steps outlined above. 
We will be using a similar methodology as telemetrix functionality is expanded. 
 