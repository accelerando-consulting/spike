
# IoTBNE Garden Spike

This is a project created for the learn-about-circuit-manufacture
workshop, first held for the Brisbane Internet of Things Interest
Group (IoTBNE).

Design and coding is
by [Christopher Biggs](http://christopher.biggs.id.au)
of [Accelerando Consulting](https://accelerando.com.au), convenor of
IoTBNE.

# About the Project

This project is a computer that lives in your garden, powered by the
sun (it also has a battery which charges during the day, and
discharges during the night).

The computer talks to

* A soil moisture sensor
* A rain/sprinkler sensor
* A temperature sensor
* A humidty sensor
* Your home WiFi
* A server on the internet that shows the measurements on your phone

All these pieces work together to give you a dashboard, right on your
iPhone or Android, of the health of your garden, right now and over
the last few weeks.

We are also going to use this project as an example, to illustrate the
journey from Problem to Product in the Internet of Things.

(**What's "The Internet of Things"?**  It's the collection of all the
computers in the world which look after Things, not People.  Our
laptop and desktop computers are for us, their main job is to show
information to our human eyes, I call this The Internet of People.   
Unlike the Internet of People, computers in the Internet of Things are
out of our sight, they talk to each other, and to servers on the
internet, but they don't often talk directly to us.  Our "Internet of
People" computers might talk to the same servers to learn about what
the IoT devices have recorded, or even send them commands.   You could
argue that intelligent systems like Alexa, Siri, Google, Cortana and
friends are bilingual, they are part IoP and part IoT.

# Getting started with the software

We are going to use a programming environment called "Arduino", named
after a pub in Italy where three Art teachers met to create a computer
that was designed for artists and non-technical creative people, not
computer nerds like me.

## Install the Arduino Integrated Development Environment (IDE)

Humans instruct computers do to things by writing Computer Programs.
This is called Programming, also known as Coding.

An IDE is a program that lets you edit computer "source code" (which
is the (mostly) human readable part of coding) and then turn it into
"machine code" that the computers understand (but is tricky for humans
to understand).  This conversion process is called "compiling".

Your instructor will provide a USB memory stick with the Arduino IDE
on it.   You can also download the Arduino IDE from [the Arduino
Download Page](https://www.arduino.cc/en/Main/Software).

## Install the ESP32 Compiler add-on for Arduino.

Arduino is both the name of the software, and a brand of computer.  
The first Arduino computers used a computer chip called the "Atmel
AVR".  We will be using a newer computer chip called the "Espressif
ESP32", which speaks a different dialect of Machine Language.
We will need to download some extra software to allow the
Arduino IDE to compile our source code into the kind of machine code
that the ESP32 understands.

Please
follow
[these instructions](https://accelerando.com.au/learn/system-on-chip/esp32/) to
install the ESP32 add-on for Arduino.

## Install the USB drivers for our ESP32 Development board

We will copy our machine code onto the ESP32's internal memory using a
USB cable.  The board will show up as a "COM Port" (aka Serial port)
on your computer when connected.  If it does not show up you will need
to install the drivers for the "CH340" USB device,
using
[these instructions](https://accelerando.com.au/learn/microcontrollers/interfacing/)

# Getting started with the hardware

Now that we have all the software we need, let's make the hardware do
our bidding.

We will begin with the "Brain" of our project, a "Development Board"
called the "TTGO Mini 32".   This board has an ESP32 processor, some
memory, a USB port, a battery charger, and some sockets for attaching
sensors.

The ESP32 microprocessor has a number of built in peripheral devices.
It has a WiFI radio, a bluetooth radio, some communication interfaces
and a number of General Purpose Input Output Pins (GPIOs).   A GPIO
pin can be programmed to **output** either High Voltage (3.3v) or Low
Voltage (0v).  We use this voltage to control outside devices.   A
GPIO pin can also be programmed to *sense* voltage, or act as an
**input**, a high voltage will be read as a 1 and a low voltage will
be read as a 0.

## Step one: make it blink

We are going to make the built-in Light Emitting Diode (LED) flash on
the board.  This is perhaps the simplest thing you can make a
microprocessor do, and it's traditionally the first program we run on
a new device, just to confirm that everything works.

Let's begin by opening the Arduino IDE.

Now select from the menu bar `File -> Examples -> 01.Basics -> Blink`.
A new window will open containing the **source code** of one of the
Arduino example programs.   This one just makes a light blink.

Many development boards have a built in Light Emitting Diode (LED) for
testing.  Your **TTGO Mini 32** development board has three

	* A red power LED that is always on
	* A blue LED that lights up when the battery is charging
	* A green LED that is connected to the ESP32's "General Purpose
      Input/Output number 22", or GPIO22.   
	  
We are going to modify this blink program to use GPIO22, by adding one
more line after line 24.

```
#define LED_BUILTIN 22
```

Lines 20-30 should thus look like

```
 This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

#define LED_BUILTIN 22
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

```

What does all this mean? [You can learn more here](https://accelerando.com.au/learn/microcontrollers/programming/).

Note, when reading the above article, that the original Arduino (with Atmel AVR microprocessor) has its
LED on GPIO13, while our ESP32 uses GPIO22.

## Step two: install the Spike program

If you are reading these instructions on Github, look at the top right
of the page for a green button marked "clone or download".   Click it,
then click "Download ZIP".    After the download completes, extract
the downloaded Zipfile.

Open up the Arduino IDE and choose `File -> Open`.   Look for the
folder you extracted (named spike) and open `spike.ino`.

Click the 'Upload' button in the Arduino IDE (second from left in the
toolbar).

After the upload completes, click the "serial monitor" button, at the far right of the
toolbar.    A window should open showing messages from the ESP32.

You should see some messages that look like

```
IoTBNE Garden Spike, v1 May 2019
#      33 NOTICE void _printWakeReason()(238) ESP Wakeup reason: other
#      33 NOTICE void setup()(186) IO Setup
```

(and many more).

## Step three: get your Spike on the WiFi

## Step four: plug in the sensors

## Oh wait, we ran out of sockets!


# Oh yuck, how do we replace that monstrosity?

# Let's design an Electronic Circuit

# Let's design a Printed Circuit Board (PCB)

# Let's get our PCB made


