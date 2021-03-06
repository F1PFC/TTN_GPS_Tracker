# TTN GPS Tracker Setup Instructions


These instructions describe the building of a GPS tracker that uses The
Things Network (TTN) to receive the trackers location. Once the location
data is received by the TTN the trackers location can be viewed on-line
on a Internet enabled device or passed to on-line mapping applications.

This project uses as its base the code used in this article; 

[http://www.thethingsnetwork.org/labs/story/lorawan-gsp-tracker](http://www.thethingsnetwork.org/labs/story/lorawan-gsp-tracker)

There are changes to the software above so that the tracker does not work (transmit) unless it has a current GPS fix. There is also an option to add a display to the tracker so that you can see the current location and other status information. The detail of how to configure TTN to accept the tracker and link it to Cayenne for mapping is explained step by step. 

The build and testing of the tracker itself is described first. Then the
configuration required for TTN is described and finally sending the
trackers location from TTN into the on-line application Cayenne is described.

Through the instructions key items to select on screens are shown highlighted in red. For some screens the values of keys have been obscured by grey boxes for security reasons. 

The tracker node is a standard Arduino device with a GPS attached. An
Arduino Pro Mini 3.3V is recommended. You could use other Arduinos such as the 5V UNO but then you will need logic level conversion for all the LoRa and GPS pins. Note that whilst the tracker software will fit in an ATmega328 based Arduino, there wont be a lot of flash or memory left for additional functions. 

The tracker software parses the location information from the GPS and sends it as latitude and longitude using the LoRa device into the TTN network. There
must therefore be a TTN gateway within reach of the tracker node. When
correctly set-up you should see the GPS location messages arriving in the
TTN console. You will need a Things network account to view this
console. You will also need to set-up the TTN console to pass your
tracker node information across to Cayenne and set up the Cayenne
desktop to receive and display the information.

The TTN tracker node program code published here is for the Arduino
environment and has been tested and runs on the Atmega328P. Remember that
the GPS and LoRa device are 3.3V logic devices, this is the reason a
3.3V Arduino Pro mini is the recommended device.

This TTN\_GPS\_Tracker uses an 868Mhz LoRa device and any of the Tracker boards that can take a Hope RFM98 device can be used, including the high altitude balloon tracker. You can find details of the Tracker boards on the 'BUY' page here;

<>

---


## The Arduino Environment and Libraries

This project does require a knowledge of how to install and use the Arduino Integrated Development Environment (IDE). You will need to know how to install it, how to load sketches, how to configure it for your Arduino board and how to install 3rd party libraries. There are numerous Arduino tutorials to be found on the Internet if your stuck.  

The tracker program itself and some of the test programs need a few third party Arduino libraries installed, these are noted in the program listings and here;

LMIC Library;   [https://github.com/matthijskooijman/arduino-lmic](https://github.com/matthijskooijman/arduino-lmic )

TinyGPSPlus Library; [https://github.com/mikalhart/TinyGPSPlus](http://github.com/mikalhart/TinyGPSPlus) 

NeoSWSerial Library; [https://github.com/SlashDevin/NeoSWSerial](https://github.com/SlashDevin/NeoSWSerial)

And if you want to add a display to the tracker you will need one of these libraries;

NewLiquidCrystal Library (version 1.3.4); [https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/](https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/)

SSD1306 Ascii Library; [https://github.com/greiman/SSD1306Ascii](https://github.com/greiman/SSD1306Ascii)



## Building the TTN GPS Tracker


Before loading and running the specific TTN\_GPS\_Tracker code on our
node it is essential to test out the separate component parts of the
tracker, specifically the GPS and the LoRa device. If you do not assure
yourself that these components are working before trying to run the
TTN\_GPS\_Tracker code, if you then have problems you will have
difficulty working out what the problems are.


## Testing the GPS


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

Some of the Tracker boards allow the
GPS to be powered off (such as the LCD receiver) if your board has this
facility you need to define the pin used to control the GPS power,
putting this pin low turns on the GPS;

\#define GPSPOWER 7 //not used pin for non Tracker boards

If your board does not have this capability define the pin as -1

Load the program and when it runs the LED should flash briefly at
start-up and then the serial monitor should show GPS output similar to
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
located indoors. The $GNGSV sentence tells you that there is one sentence of $GNGSV information and that one GPS satellite has been detected.

Whilst the above shows the GPS is working it has yet to get a location
fix. It is very important to check that the GPS does get a fix and that the
GPS reports the location data. So take the GPS outdoors and monitor the
serial monitor. Within a couple of minutes the GPS output should look
like this;

GPS with fix output.

Note that a lot of the Ublox GPSs default to the $GNGGA and $GNRMC
NMEA sentence format, other GPSs may use $GPGGA and $GPRMC formats.
The GPS library used in the TTN\_GPS\_Tracker code recognises both types.

## Testing the LoRa device


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

    |   Arduino   | LoRa Device |
    | ----------- | ----------- | 
    |     VCC     |    3.3V     |  
    |     GND     |    GND      |
    |   SCK (13)  |    SCK      | 
    |  MISO (12)  |    MISO     |
    |  MOSI (11)  |    MOSI     |


Pro Mini pin numbers shown in brackets. Connections to the LoRa devices
DIO0, DIO1 and DIO2 pins are not required for this test.

Program your board with the LoRa\_Module\_Test\_868Mhz.ino program and
when it runs the Arduino serial monitor output should show the
following;

    LoRa_Module_Test_868Mhz
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
transmits an FM tone. You could use a software defined radio (SDR) to listen for the tone.

## Setting up the TTN Console 

With GPS and LoRa device working on our board we can move on to setting
up the TTN console to accept the packets the TTN\_GPS\_Tracker will be
transmitting.

The following instructions are based in part on this video, which is a
simple to follow set of instructions for setting up a TTN node, I
recommend you watch it;

<https://www.youtube.com/watch?v=duwUwXt-hs8>

## Creating a Things Network Account (if you do not already have one)


Goto the Things Network website;  <https://www.thethingsnetwork.org/>

And select the 'Sign Up' button at the top of the screen;

<br><br>
<img src="/Pictures/2.jpg" width="650"/>
<br><br>

Provide the details for your new account, you will need an email
address;

<img src="/Pictures/3A.jpg" width="650"/>

<br><br>
You will get an email with a link to confirm your account.

## Log into your Things Network Account

Goto the Things Network Website <https://www.thethingsnetwork.org/>

And select the login button, top and far right;
<br><br>
<img src="/Pictures/4.jpg" width="650"/>
<br><br>
Once you have logged in drop down the menu against your name and select
Console;

<br><br>
<img src="/Pictures/4A.jpg" width="650"/>
<br><br>

## Configuring applications in the TTN console

Select applications;
<br><br>
<img src="/Pictures/4B.jpg" width="650"/>
<br><br>
If you have created an application before you will see a list of them,
otherwise you just see a screen like below. 
<br><br>
<img src="/Pictures/4C.jpg" width="650"/>
<br><br>

Select 'Add Application'. 
Give your application a name, but note these limitations; application ID
can only consist of lowercase alphanumeric characters, with no
consecutive -- or \_ characters and it cannot start or end with a - or a
\_ .

We call our application; 'ttn\_gps\_tracker'
<br><br>
<img src="/Pictures/5.jpg" width="650"/>
<br><br>

Ensure that the **ttn-handler-eu** is selected in the 'Handler registration'
box.

When you have completed the form, click on 'Add application', at the bottom of the screen and you
will see the application overview.

<br><br>
<img src="/Pictures/8.jpg" width="650"/>
<br><br>

## Configuring Devices in the TTN console

Since this is a new application there are no devices yet configured for the application, so we need to add
one, click on devices at the top of the screen;


<br><br>
<img src="/Pictures/9.jpg" width="650"/>
<br><br>



Then click on 'register device'. 

<br><br>
<img src="/Pictures/10.jpg" width="650"/>
<br><br>

You will see the register device screen;
<br><br>
<img src="/Pictures/12.jpg" width="650"/>
<br><br>


Provide a name for the Device, such as
'ttn\_gps\_tracker\_1' and enter it in the Device ID box. 

You will see the register device screen;
<br><br>
<img src="/Pictures/13.jpg" width="650"/>
<br><br>


Enter a Device EUI such as 00 11 22 33 44 55 66 77. 

<br><br>
<img src="/Pictures/14.jpg" width="650"/>
<br><br>

Then click on 'Register' bottom right of the screen;

<br><br>
<img src="/Pictures/15.jpg" width="650"/>
<br><br>


Note that the Device EUI needs to be unique and we will need to use it later. You can enter any number you like, its in hexadecimal, so 00 11 AA BB CC DD EE FF would also be acceptable. If the number is already in use you will get a warning;  

<br><br>
<img src="/Pictures/14A.jpg" width="650"/>
<br><br>

If the Device EUI is unique it will be accepted and we should now have the **DEVICE OVERVIEW** screen;

<br><br>
<img src="/Pictures/16.jpg" width="650"/>
<br><br>

This is the initial default device overview and assumes we will be using OTAA (over the air authentication) but we want to use ABP (activation by personalisation), so we need to make a change. Click on settings from the selection at the top of the device overview
screen;

<br><br>
<img src="/Pictures/17.jpg" width="650"/>
<br><br>

When the settings screen appears find the Activation Method box and
ensure ABP is selected and highlighted. 

<br><br>
<img src="/Pictures/18.jpg" width="650"/>
<br><br>

Then click 'Save' at the very bottom of the screen. Note that the **DEVICE OVERVIEW** screen has now changed;

<br><br>
<img src="/Pictures/18A.jpg" width="650"/>
<br><br>

There is now a 'Network Session Key' and 'App Session Key' boxes displayed. We need to copy these values into our Arduino program. 

Click on the eye button (highlighted) to see the actual key and the \<\> (highlighted) to see the keys in a format that we will need to copy into the Arduino TTN\_Node\_Test program. Click on the button to the right of the Key (highlighted), it will copy the Respective key into the clipboard so that we can then paste it into the program.

Open the TTN\_GPS\_Tracker.ino program in the Arduino IDE and find these
lines in the All\_Configuration.h file, section '2) Program Options';

**NWKSKEY**\[16\] =
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

**APPSKEY**\[16\] =
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

**DEVADDR** = 0x00000000;

Paste the Network Session Key from the TTN console into the NWKSKEY
definition and the App Session Key into to the APPSKEY definition within
the configuration file.

Be careful when copying these two keys not to create double sets of curly braces in the configuration file such as;

u1\_t NWKSKEY\[16\] = **{{**  0x27, 0x55, 0xF2,
                         

The console will also have generated a device address so we need to copy the Device address into the DEVADDR definition into the configuration file. The format of this number will need changing to reflect the format the Arduino program expects. Make the changes so that the spaces are removed and there is a '0x' in front of the number. For instance of the number on the 'DEVICE OVERVIEW screen looks like this

 11 22 33 AA 

you need to change is so that the relevant line in the All\_Configuration.h file look like this;

DEVADDR = 0x112233AA;  

This **DEVICE OVERVIEW** screen also has the Device EUI displayed on it, we will need it later. Select to copy it into the clipboard as for the previous keys and paste it into a text editor or similar and save it for later. You could even add it the the configuration file like this;

//00 11 22 33 44 55 66 77

The Arduino IDE will see the //characters as a comment and ignore the rest of the line.  

You have completed your node configuration. You can now load the tracker program and test it.


## Loading the TTN GPS Tracker program


With the NWKSKEY, APPSKEY and DEVADDR entered into the All\_Configuration.h
file load the TTN\_GPS\_Tracker program onto your board. There are some options that can be selected to use a GPS test location and add a display to the tracker, but these are turned off by default, see the section on 'Program Options' later on if you want to use these functions.

## Running the tracker program

With the tracker program loaded and power applied to the tracker, observe the startup messages in the Arduino IDE serial monitor, you should see something like this;


    TTN_GPS_TRACKER
    101018
    Stuart Robinson
    
    Waiting for Startup GPS Fix


Note that the program has stopped waiting for the GPS to get a fix, and the LED is on. When the tracker is waiting for a GPS fix for the first time after power up the LED will blink twice approximately every 5 seconds.  This wait is by design, we do not want the tracker sending erroneous GPS information if it is not working properly. With the GPS outside with a good view of the sky the GPS should get a fix, the LED will go off and the serial monitor should report something like this;


    TTN_GPS_TRACKER
    101018
    Stuart Robinson
    
    Waiting for Startup GPS Fix
    Waiting for GPS Fix - Have GPS Fix
    Packet queued
    401515: EV_TXCOMPLETE


To see if the location data from your tracker is getting into  the Things Network, login to your TTN account and goto the console. Select
applications and then (for this example) select the TTN\_GPS\_Tracker\_application, you should see this screen;

<br><br>
<img src="/Pictures/25.jpg" width="650"/>
<br><br>


Click on 'Data' on the top right of the window and you should see the
Application data packets arriving, click on a line to expand and show the data for a packet;


<br><br>
<img src="/Pictures/25A.jpg" width="650"/>
<br><br>



## Create a Cayenne Account


Goto [myDevices](https://accounts.mydevices.com/auth/realms/cayenne/protocol/openid-connect/auth?response_type=code&scope=email+profile&client_id=cayenne-web-app&state=dzynyCVzyKYs3A4ms2ve1MFLPui62Cp9guINWH5l&redirect_uri=https%3A%2F%2Fcayenne.mydevices.com%2Fauth%2Fcallback) and select login to an existing account or 'Sign Up' to create an account. 

<br><br>
<img src="/Pictures/26.jpg" width="650"/>
<br><br>

If this is a new account, you will have no devices configured so select  'All Devices' from the screen below;

<br><br>
<img src="/Pictures/29.jpg" width="650"/>
<br><br>

If you already have an account the login and you should go to the myDevices dashboard and select 'Add new' on the top left of the dashboard, then select 'Device widget'.   

Then scroll down till you find the 'The Things Network' and select it

<br><br>
<img src="/Pictures/31.jpg" width="650"/>
<br><br>

You should see the 'Devices and Widgets' screen;

<br><br>
<img src="/Pictures/28.jpg" width="650"/>
<br><br>

Type LPP into the search box to and select the Cayenne LPP widget 

<br><br>
<img src="/Pictures/30.jpg" width="650"/>
<br><br>

For the next screen get the DevEUI we saved earlier and enter it in the DevEUI box, then select 'ADD Device'   


<br><br>
<img src="/Pictures/31A.jpg" width="650"/>
<br><br>

If you get a 'Conflict' Error then the DevEUI your using is not unique, you need to go back to the TTN console and select another. If all goes well you should see a blank dashboard;

<br><br>
<img src="/Pictures/31B.jpg" width="650"/>
<br><br>

Note the URL of this screen contains a number, highlighted in red, the number from the screen in this case is;

9156d9e0-aaaa-11e8-9c33-7336b356aaaa 

Your number will obviously be different. You need to copy this and save it, we need to enter it into the TTN console. 


## Enable TTN Cayenne Integration

Log back into your TTN console and go to Application overview and then select 'Integrations' 

<br><br>
<img src="/Pictures/33.jpg" width="650"/>
<br><br>

Select 'Add Integration'

<br><br>
<img src="/Pictures/34.jpg" width="650"/>
<br><br>

Select 'Cayenne'

<br><br>
<img src="/Pictures/35.jpg" width="650"/>
<br><br>

For the 'Process ID' enter the number copied from the Cayenne dashboard URL earlier, and click on 'default key' for the Access key. Then click on 'Add Integration'

<br><br>
<img src="/Pictures/36.jpg" width="650"/>
<br><br>

And you should see the Integration overview screen show a status of 'Running'

<br><br>
<img src="/Pictures/37.jpg" width="650"/>
<br><br>

Go back to the Cayenne dashboard and with in a few seconds of the packet being sent a map appears on the dashboard with the location of the tracker;

<br><br>
<img src="/Pictures/38.jpg" width="650"/>
<br><br>

If you don't see the trackers location on the map drop down the Cayenne LPP box just below 'Add new' button at the top left of the screen and select GPS from the list under Cayenne LPP. 

To simulate the effect of a moving balloon tracker I programmed the tracker, still in my workshop, to send a test series of location packets, see below;


<br><br>
<img src="/Pictures/39.jpg" width="650"/>
<br><br>


## Program Options - TTN GPS Tracker program

There are changes that can be made to the programs functions and options by making changes to the 'All\_Configuration.h' file before compiling and loading the program into the tracker, a full description of the changes you can make in the configuration file is listed below in the 'Using the All\_Configuration File', but some of the basic changes are described here in brief. 


###Display option

The TTN\_GPS\_Tracker program has the option to display
information on a local display connected to the Arduino device via an I2C connection, either a 20x4 LCD display or a SSD1306 OLED. The option to enable the display is turned off by default by this line in the All\_Configuration.h file;

    //#define Use_Display

The two // characters turn the text that follows into a comment which is
ignored by the Arduino compiler. To enable the display option you need to remove the two // characters in front of the define line. 

Do not enable the display option unless the appropriate display is attached
and working. The displays need to be 3.3V versions. The 20x4 LCD is connected via an I2C PCF8574 I\O expander board specifically designed for driving the HD44780 controller on the LCD. 


### Frame Counter

Each packet or payload that the node sends has a frame counter and the TTN
network keeps track of them. If for example your node had sent 20
packets or payloads (frame counters 0-19) and you restart your node then
the TTN network will ignore the packets until the frame counter reaches
20, then it will continue to display the payloads in the console again.

This behaviour can give the impression that your node is not working.
You can disable this behaviour for testing purposes. Go to the
application overview screen, select 'Devices' then select the device
to configure and then from the 'Device Overview' screen select 'Settings' on the top right.

At the bottom of the screen of device settings you will see a box labelled 'frame counter settings'

You can un-tick this box, and when the node resets the frame counter will
start from zero and the packets or payloads will not now be rejected. This
is OK when testing a node, but do not leave it permanently set.

### A warning note about batteries

If your tracker is small you may be tempted to use a small battery, in particular you may be tempted to use a Lithium Polymer (Lipo) battery. Be very very careful if you use a Lipo. These batteries in particular can be dangerous when abused and yes they really can explode. In a tracker its quite likely that at some point the tracker will be left on and the battery will go flat. If this happens to a Lipo then consider binning it, you cannot be sure if it's suffered internal damage. Some, but not all, Lipos have internal circuits that disconnect the battery if it's voltage goes too low but unfortunately this can be at 2.4V, a bit too low really. 

For long life and continued safety a Lipo powered project should turn itself off when the battery goes below around 3.0V, but very few DIY projects have this capability built in. Of course for the obvious safety reasons most commercial applications that use Lipos, such a mobile phones, tablets, laptop PCs etc, do have this protection built in. 

A safer alternative to Lipos is to use say three or four AAA or AA alkaline batteries, these are easy to get and low cost. You can also use NiMh rechargeable versions of the AAA or AA batteries. Another alternative is to use a single LiFePO4 battery, these will supply around 3.3V to a project so they can be connected directly to the VCC of a project and no regulator is needed. The LiFePO4 batteries do not have the same fire risks as Lipos.      


## Using the All\_Configuration File 

This document describes how to use the configuration file for the TTN\_GPS\_TRACKER program file. The program file is for the Arduino integrated development environment (IDE). The configuration file has been created so that all configuration for the program and any changes needed are carried out by editing this file. When you load the  program file in the Arduino IDE the All\_Configuration.h file should appear on the second tab from the left in the IDE. You should not need to make changes to the program file itself. 
The configuration file uses a series of #defines and constant definitions that the compiler recognises and uses when building (compiling)  the working program. In general the defines are either commented in or commented out, an example will explain, for instance look at these lines from the 1) Hardware related definitions and options section of the settings file;

    #define Board_Definition "HAB3_Board_Definitions.h"   
    //#define Board_Definition "LCD_Receiver_Board_Definitions.h"  
    //#define Board_Definition "Pro_Mini_Mikrobus_Shield_Board_Definitions.h" 
    //#define Board_Definition "Locator2_Board_Definitions.h"
    //#define Board_Definition "Custom_Board_Definitions.h" 


The first #define Board\_Definition is commented in and within the program itself there is a bit of code that says include the hardware definitions file HAB3\_Board\_Definitions.h when the program is complied. This hardware definitions file contains the required pin definitions for the Tracker HAB3 board in use.

If we want to compile the program for a different LoRaTraker board, say the LCD Receiver board, we would comment out the HAB3\_Board\_Definitions.h file by adding two // characters at the start of its line and then removing the two // characters at the start of the LCD\_Receive\r_Board\_Definitions.h line. The lines would then look like this;

    //#define Board_Definition "HAB3_Board_Definitions.h"   
    #define Board_Definition "LCD_Receiver_Board_Definitions.h"  
    //#define Board_Definition Pro_Mini_Mikrobus_Shield_Board_Definitions.h" 
    //#define Board_Definition "Locator2_Board_Definitions.h"
    //#define Board_Definition "Custom_Board_Definitions.h"  

You can use the TTN\_GPS\_TRACKER program for other Arduino hardware setups (i.e. not a Tracker board) but you will need to edit the Custom\_Board\_Definitions.h file to match your own boards pin connections and then #include it as described above.  

Some parameters are set with constant definitions. For example from GPS section of the All\_Configuration file. 


    static const  uint16_t  GPSBaud = 9600;//GPS baud rate 

This constant sets the baud rate for the GPS you are using. Each section of the configuration file is now described in turn.

### 1) Hardware related definitions and options 

There is a definition file for each of the Tracker board types, these definition files specify which pins on the board perform a particular function for instance most all definition files have a line like this;

    #define LED1 8

This means the board LED is on pin 8. Thus when the program turns on the LED such as with this line;


    digital.Write(LED1, HIGH);

The program knows to set pin 8 high, which turns on the LED.

Using pin definitions in this way makes the programs easier to understand and adapt for different hardware setups. 

The  TTN\_GPS\_TRACKER\_CayenneLPP program has 4 pre-made definition files for the Tracker boards;

    LCD_Receiver_Board_Definitions.h
    HAB3_Board_Definitions.h
    Pro_Mini_Mikrobus_Shield_Board_Definitions.h
    Custom_Board_Definitions.h

The Custom\_Board\_Definitions.h file allows you to create the pin allocations for a non Tracker board. 

Within Section 1) to define which board definition file to use, remove the two // characters in front of the # define line for the board you are using. Only ‘comment in’ the define for one of the hardware definition files. 

Section 1) of the All\_Configuration.h file also contains these two lines;

    #define LoRa_Device_in_MB1 
    #define GPS_in_MB2                                                      

Some Tracker boards, such as the  LCD\_Receiver\_Board use Mikrobus modules for the LoRa device and GPS, The two  #define lines above allocate the appropriate pin definitions depending on which Mikrobus socket the LoRa module and GPS is in. The LoRa device is normally in MB1 and the GPS in MB2. The HAB3 board does not use Mikrobus sockets. 

 
### 2) Program Options - Keys for LoRaWAN etc.
This is the section where you enter the keys for the TTN connection, NWKSKEY the network session key,   APPSKEY the application key and  DEVADDR the device address. 

When entering the keys be sure that the format remains the same, all the 0x00 sections are replaced with the values of your keys and are in the same format. 

This section also contains the nominal transmission (into the TTN) interval. The actual interval will be about 5 seconds longer. 

  

### 3) GPS Options
Define the GPS baud rate here, often it will be 9600 baud, but not always.
    
    static const uint32_t GPSBaud = 9600;            

For test purposes the program can use a defined test GPS location for transmission rather than the real current location obtained from the GPS, this can be useful for testing purposes and if you don’t want to reveal your true location whilst testing.  

To use the test location uncomment this define;

    //#define Use_Test_Location 

And change the test location if you choose to by editing these lines. 

    //Pen-y-Fan summit, Brecon Beacons
    #define TestLatitude 51.88406
    #define TestLongitude -3.43644
    #define TestAltitude 886


### 4) Display Settings
The  TTN\_GPS\_TRACKER program has an option to attach a I2C display, either a SSD1306 or a 20x4 LCD based using the HD44780 controller and fitted with a PCF8574 I2C driver board. 
To use either display option enable this define by un-commenting it;

    //#define Use_Display

Then enable the define for one of the appropriate display libraries;


    #define Display_Library "Display_I2C_LCD2.h"   //20x4 LCD display with PCF8574 backpack    
    or
    #define Display_Library "Display_SD1306_AVR.h" //use an SSD1306 I2C display

How the GPS information from the tracker program is displayed on screen is controlled by defining the appropriate display screens library. There are different screen libraries as the two displays used have different numbers of lines and characters.   


    #define Display_Screens "I2C_LCD_20x4_Screens.h"   //use the 20x4 LCD Screens     
    or    
    #define Display_Screens "SD1306_SMALL_TEXT_Screens.h"
    
The two I2C attached displays need an I2C address defined, they can vary from standard, the addresses are defined here;

    const int PCF8574Address = 0x27;   //I2C address of the PCF8574 for 20x4 LCD
    or    
    const int I2C_ADDRESS = 0x3C;   //I2C address of the SD13206
    
If your unsure of the I2C address of the screen you have attached, run the I2C\_Scanner\_Test program that is in the \Programs directory on the repository downloaded from GITHUB.

Some temporary screens are used to alert the user to some condition, the period these screens are displayed in mS is defined here;


    const unsigned int screen_delaymS = 2000; 

### Note:
Whilst you can do some debugging, the GPS tests for instance with just the USB serial programming adapter powering the your board, do not be surprised if the board resets when the LoRa device starts to transmit.  


### Good Luck.

<br><br>
  

## Stuart Robinson
## October 2018
