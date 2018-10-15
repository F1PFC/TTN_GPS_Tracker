#TTN\_GPS\_Tracker\_Setup\_Instructions


These instructions describe the building of a GPS tracker that uses The
Things Network (TTN) to receive the trackers location. Once the location
data is received by the TTN the trackers location can be viewed online
on a Internet enabled device or passed to on-line mapping applications.

The build and testing of the tracker itself is described first. Then the
configuration required for TTN is described and finally sending the
trackers location from TTN into an online application, Cayenne in this
case, is described.

The tracker node is a simple Arduino device with a GPS attached. An
Arduino Pro Mini 3.3V is all that is required. The tracker software
parses the location information from the GPS and sends the location as
latitude and longitude using the LoRa device into the TTN network. There
must therefore be a TTN gateway within reach of the tracker node. When
correctly setup you should see the GPS location messages arriving in the
TTN console. You will need a Things network account to view this
console. You will also need to setup the TTN console to pass your
tracker node information across to Cayenne and set up the Cayenne
desktop to receive and display the information.

The TTN tracker node program code published here is for the Arduino
environment and has been tested and runs on the Atmega328P. Note that
the GPS and LoRa device are 3.3V logic devices, this is the reason a
3.3V Arduino Pro mini is the recommended device.

##Building the TTN\_GPS\_Tracker


Before loading and running the specific TTN\_GPS\_Tracker code on our
node it is essential to test out the seperate component parts of the
tracker, specifically the GPS and the LoRa device. If you do not assure
yourself that these components are working before trying to run the
TTN\_GPS\_Tracker code, if you then have problems you will have
difficulty working out what the problems are.

###Testing the GPS


A very common problem reported with GPS trackers is that 'the GPS does
not work'. The TTN\_GPS\_Tracker code will not work without a
functioning GPS so it is essential to test it first.

Load the EchoGPS\_Softwareserial\_Test.ino program into the Arduino IDE,
makes sure the Arduino IDE is set to use the Arduino Pro Mini at 3.3V
8Mhz, see below;  

![Picture 1](/Pictures/1.jpg)


The EchoGPS\_Softwareserial\_Test uses the softwareserial library
program to read the GPS, you need to define the pins that the GPS is
connected to, see these two defines at the start of the Echo GPS test program;

\#define GPSTX A2 //pin number for TX output - Arduino into GPS

\#define GPSRX A3 //pin number for RX input - To Arduino from GPS

Also define the pin number that the LED is connected to, it will be used
by the TTN\_GPS\_Tracker code. 

Some of the LoRaTracker boards allow the
GPS to be powered off (such as the LCD receiver) if your board has this
facility you need to define the pin used to control the GPS power,
putting this pin low turns on the GPS;

\#define GPSPOWER 7 //not used pin for non LoRaTracker boards

If your board does not have this capability define the pin as -1

Load the program and when it runs the LED should flash briefly at
startup and then the serial monitor should show GPS output similar to
this;

        EchoGPS_Softwareserial_Test
		10/10/2018
        Stuart Robinson
        $GNVTG,,,,,,,,,N\*2E
        $GNRMC,102208.00,V,,,,,,,121018,,,N\*61
        $GNVTG,,,,,,,,,N\*2E
        $GNGGA,102208.00,,,,,0,00,99.99,,,,,,\*71
        $GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99\*2E
        $GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99\*2E
        $GPGSV,1,1,02,08,,,25,22,,,34\*73
        $GLGSV,1,1,00\*65
        $GNGLL,,,,,102208.00,V,N\*5D
        $GNRMC,102209.00,V,,,,,,,121018,,,N\*60
        $GNVTG,,,,,,,,,N\*2E
        $GNGGA,102209.00,,,,,0,00,99.99,,,,,,\*70
        $GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99\*2E
        $GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99\*2E
        $GPGSV,1,1,02,08,,,25,22,,,35\*72
        $GLGSV,1,1,00\*65
        $GNGLL,,,,,102209.00,V,N\*5C
        $GNRMC,102210.00,V,,,,,,,121018,,,N\*68
        $GNVTG,,,,,,,,,N\*2E
        $GNGGA,102210.00,,,,,0,00,99.99,,,,,,\*78
        $GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99\*2E
        $GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99\*2E
        $GPGSV,1,1,02,08,,,25,22,,,35\*72
        $GLGSV,1,1,00\*65
        $GNGLL,,,,,102210.00,V,N\*54

Note that the two NMEA sentences that normally contain the location
data, $GNGGA and $GNRMC are empty of the latitude and longitude data.
This is normal for a GPS that has just been powered up or that is
located indoors. The $GNGSV senetence tells you that there is one sentence of $GNGSV information and that one GPS satellite has been detected.

Whilst the above shows the GPS is working it has yet to get a location
fix. It is very important to check that the GPS does get a fix and that the
GPS reports the location data. So take the GPS outdoors and monitor the
serial monitor. Within a couple of minutes the GPS output should look
like this;

GPS with fix output.

Note that a lot of the Ublox GPSs default to the $GNGGA and $GNRMC
NMEA sentence format, other GPSs may use $GPGGA and $GPRMC formats.
The GPS library used in the TTN\_GPS\_Tracker code recognises both types.

###Testing the LoRa device


It is also important to test that the LoRa device is connected correctly
working before running the TTN\_GPS\_Tracker code.

Load the LoRa\_Module\_Test\_868Mhz.ino program into the Arduino IDE, do
not program the board yet. Your board needs to have the following pins
defined in the program and connected to the LoRa device;

\#define lora\_NSS 10 //Arduino pin number for NSS on LoRa device

\#define lora\_NReset 9 //Arduino pin number for RESET pin on LoRa
device

\#define LED1 8 //Arduino pin number for LED, when high LED is on.

You also need to connect these pins;

Arduino LoRa Device

VCC 3.3V

GND GND

SCK (13) SCK

MISO (12) MISO

MOSI (11) MOSI

Pro Mini pin numbers shown in brackets. Connections to thew LoRa devices
DIO0, DIO1 and DIO2 pins are not required for this test.

Program your board with the LoRa\_Module\_Test\_868Mhz.ino program and
when it runs the Arduino serial monitor output should show the
following;

    LoRa\_Module\_Test\_868Mhz
    V1.1
    10/10/2018
    Stuart Robinson
    LED Flash
    Checking LoRa Device
   
    Registers after reset - LoRa Registers
    Reg 0 1 2 3 4 5 6 7 8 9 A B C D E F
    0x00 00 0F 1A 0B 00 52 6C 80 00 4F 09 2B 20 08 02 0A
    0x10 FF 6F 15 0B 28 0C 12 47 32 3E 00 00 00 00 00 40
    0x20 00 00 00 00 05 00 03 93 55 55 55 55 55 55 55 55
    0x30 90 40 40 00 00 0F 00 00 00 F5 20 82 FD 02 80 40
    0x40 00 00 12 24 2D 00 03 00 04 23 00 09 05 84 32 2B
    0x50 14 00 00 11 00 00 00 0F E0 00 0C FD 07 00 5C 78
    0x60 00 19 0C 4B CC 0E 7F 20 04 47 AF 3F B2 00 37 0B
    0x70 D0 01 10 00 00 00 00 00 00 00 00 00 00 00 00 00
    Frequency after reset - 434.000Mhz
    Device Present
    
    Registers after setup - LoRa Registers
    Reg 0 1 2 3 4 5 6 7 8 9 A B C D E F
    0x00 00 0F 1A 0B 00 52 6C 80 00 4F 09 2B 23 08 02 0A
    0x10 FF 00 15 0B 28 0C 12 47 32 3E 00 00 00 00 00 FF
    0x20 00 0C 00 00 05 00 03 93 55 55 55 55 55 55 55 55
    0x30 90 40 40 00 00 0F 00 00 00 F5 20 82 FD 02 00 40
    0x40 00 00 12 24 2D 00 03 00 04 23 00 09 05 84 32 2B
    0x50 14 00 00 11 00 00 00 0F E0 00 0C 00 07 00 5C 78
    0x60 00 19 0C 4B CC 0E 41 20 04 47 AF 3F B4 00 38 0B
    0x70 D0 01 10 00 00 00 00 00 00 00 00 00 00 00 00 00
    New Frequency - 868.000Mhz
    Transmit FM Tone - Done

Note that at reset the default LoRa device frequency is 434.000Mhz. The program changes the frequency to 868.000Mhz (shown as New Frequency) and
transmits an FM tone. You could use an SDR to listen for the tone.

##Setting up the TTN Console 


With GPS and LoRa device working on our board we can move on to setting
up the TTN console to accept the packets the TTN\_GPS\_Tracker will be
transmitting.

The following instructions are based in part on this video, which is a
simple to follow set of instructions for setting up a TTN node, I
recommend you watch it;

<https://www.youtube.com/watch?v=duwUwXt-hs8>

###Create a Things Network Account (if you do not already have one)
-

Goto the Things Network website;

 <https://www.thethingsnetwork.org/>

And select the 'Sign Up' button at the top of the screen;

<br><br>
<img src="/Pictures/2.jpg" width="450"/>
<br><br>

Provide the details for your new account, you will need an email
address;

<img src="/Pictures/3A.jpg" width="450"/>

<br><br>
You will get an email with a link to confirm your account.

###Log into your Things Network Account

Goto the Things Network Website

<https://www.thethingsnetwork.org/>

And select the login button, top and far right;
<br><br>
<img src="/Pictures/4.jpg" width="450"/>
<br><br>
Once you have logged in drop down the menu against your name and select
Console;

<img src="/Pictures/4A.jpg" width="450"/>
<br><br>
###Configuring applications in the TTN console

Select applications;
<br><br>
<img src="/Pictures/4B.jpg" width="450"/>
<br><br>
If you have created an application before you will see a list of them,
otherwise you just see a screen like below. 
<br><br>
![Picture 4C](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/4C.jpg)
<br><br>

Select 'Add Application'. 
Give your application a name, but note these limitations; application ID
can only consist of lowercase alphanumeric characters, with no
consecutive -- or \_ characters and it cannot start or end with a - or a
\_ .

We call our application; 'TTN\_GPS\_Tracker'

![Picture 5](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/5.jpg)


Ensure that the ttn-handler-eu is selected in the 'Handler registration'
box.

When you have completed the form, click on 'Add application', at the bottom of the screen and you
will see the application overview.

![Picture 8](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/8.jpg)

###Configuring Devices in the TTN console

Since this is a new application there are no devices yet configured for the application, so we need to add
one, click on devices at the top of the screen;


![Picture 9](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/9.jpg)



Then click on 'register device'. 

![Picture 10](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/10.jpg)

You will see the register device screen;

![Picture 12](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/12.jpg)


Provide a name for the Device, such as
'ttn\_gps\_tracker\_1' and enter it in the Device ID box. 

![Picture 13](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/13.jpg)



Enter a Device EUI such as 00 11 22 33 44 55 66 77. This device EUI needs to be unique and we will need it later. You can enter any number you like, its in hexadecimal, but if the number already exists you will get a warning.  

![Picture 14](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/14.jpg)

Then
click on 'Register' bottom right of the screen;

![Picture 15](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/15.jpg)

We should then see the device overview screen;

![Picture 16](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/16.jpg)


Click on settings from the selection at the top of the device overview
screen;

![Picture 17](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/17.jpg)


When the settings screen appears find the Activation Method box and
ensure ABP is selected and highlighted. 


![Picture 18](https://raw.github.com/LoRaTracker/TTN_GPS_Tracker/master/Pictures/18.jpg)


Then click 'Save' at the bottom
of the screen.

We are using ABP activation so we need to copy the 'Network Session Key'
and 'App Session Key' into our Arduino program. Then Key box initially
looks like this;

Click on the eye button to see the actual key and the \<\> to see the
app key in a format that we will need to copy into the Arduino
TTN\_Node\_Test program. Click on the button to the right of the Key, it
will copy the Respective key into the clipboard so that we can then
paste it into the program.

Open the TTN\_GPS\_Tracker.ino program in the Arduino IDE and find these
lines in the configuration.h file, section '2) Program Options';

**NWKSKEY**\[16\] =
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

**APPSKEY**\[16\] =
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

**DEVADDR** = 0x00000000;

Paste the Network Session Key from the TTN console into the NWKSKEY
definition and the App Session Key into to the APPSKEY definition within
the configuration file.

The console will also have generated a device address so copy Device
address into the DEVADDR definition into the configuration file.

The completed entries should look like this, note the 0x added in front
of the number and the removed spaces;

(for security reasons the actual key shown is not valid)

Be careful when copying not to create double sets of curly braces in the
configuration file such as;

u1\_t NWKSKEY\[16\] =** { {** 0x27, 0x55, 0xF2,

2\) Program Options

##Loading the TTN\_GPS\_Tracker program


With the NWKSKEY, APPSKEY and DEVADDR entered into the configuration.h
file load the program onto your board.

To see if the data your node is sending is going through the Things
Network, login to your TTN account and goto the console. Select
applications and then (for this example) select the
loratracker\_test\_application byt clicking on the coloured button;

You should see this screen;

Click on 'Data' on the top right of the window and you should see the
Application data packets arriving;

The payload is displayed in hexadecimal. 0X4C is ASCII 'L' and 0x6F is
ASCII 'o' etc.

Note - Frame Counter

Each packet\\payload that the node sends has a frame counter and the TTN
network keeps track of them. If for example your node had sent 20
packets\\payloads (frame counters 0-19) and you restart your node then
the TTN network will ignore the packets until the frame counter reaches
20, then it will continue to display the payloads in the console again.

This behaviour can give the impression that your node is not working.
You can disable this beahviour for testing purposes, by going to the
Application overview screen, selecting 'Devices' then select the device
to configure and then from the 'Device Overview'

And then select 'Settings' on the top right.

At the bottom of the screen of device settings you will see this box,
'frame counter settings'

You can untick this box, and when the node resets the frame counter will
start from zero and the packets\\payloads will not now be rejected. This
is OK when testing a node, but do not leave it permanently set.

You have completed your node configuration. You can now carry out a test
of the TTN\_GPS\_Tracker node.


##Create a Cayenne Account


Goto http://www.mydevices.com\

##TTN\_GPS\_Tracker program -- Display option


Note that the TTN\_GPS\_Tracker program has the option to display
information on a local display, either a 20x4 LCD display or a SSD1306
OLED, this option is turned off by default because of this line in the
configuration file;

//\#define Use\_Display

The two // characters turn the text that follows into a comment which is
ignored by the Arduino compiler. To enable the display option we would
change the line into;

\#define Use\_Display

Do not enable the display option unless a display is actually attached
and working.

Stuart Robinson

June 2018
