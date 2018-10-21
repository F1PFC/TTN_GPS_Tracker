#define programname "EchoGPS_Softwareserial_Test"
#define dateproduced "10/10/2018"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>
#define GPS_in_MB2                                   //required if board is using Mikrobus sockets

/*
*****************************************************************************************************************************
LoRaTracker Test Programs

Copyright of the author Stuart Robinson - 28/11/2017

http://www.LoRaTracker.uk

These programs may be used free of charge for personal, recreational and educational purposes only.

This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
of the author Stuart Robinson.

The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
free from errors.
*****************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Program operation

The purpose of this program is to check that a Serial GPS is working. Characters are read from the GPS at 9600 baud (default) 
using software serial and sent to the Arduino IDE Serial Monitor at 115200 baud (default).
********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Connections

The program uses two pins for the software serial interface on the Arduino. You can explicitly define the required pins below
by removing the two // characters in front of the #defines 
********************************************************************************************************************************
*/

#define GPSTX A3                           //pin number for TX output - Arduino into GPS
#define GPSRX A2                           //pin number for RX input - To Arduino from GPS
#define GPSPOWER 7                         //not used pin for non LoRaTracker boards so define as -1 or use 7 for Locator2 boards
#define LED1 8                             //pin number for LED                          

/*
***********************************************************************************************************************************************
Board Definitions

As an alternative to explicitly defining the Arduino pins required, there are pre-defined board definition files for the LoRaTracker boards
included in the LoRaTracker Library;

HTTPS://github.com/LoRaTracker/LoRaTracker-Library

Select (include) the board definition file you require by removing the // characters before the appropriate include line in the list below
***********************************************************************************************************************************************
*/

//#include "LCD_Receiver_Board_Definitions.h"


#define GPSBaud 9600                         //baud rate for GPS 
#define Serial_Monitor_Baud 115200           //this is baud rate used for the Arduino IDE Serial Monitor


#include <SoftwareSerial.h>                     
SoftwareSerial GPSserial(GPSRX, GPSTX);        


void loop()                    
{
  while (GPSserial.available() > 0)
  Serial.write(GPSserial.read());
}


void led_Flash(unsigned int flashes, unsigned int delaymS)
{
  //flash LED to board is alive
  unsigned int index;

  for (index = 1; index <= flashes; index++)
  {
    digitalWrite(LED1, HIGH);
    delay(delaymS);
    digitalWrite(LED1, LOW);
    delay(delaymS);
  }
}


void setup()
{
  Serial.begin(Serial_Monitor_Baud);         //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();
  
  pinMode(LED1, OUTPUT);                     //setup pin for LED
  pinMode(GPSPOWER, OUTPUT);                 //setup pin for GPS Power Control, in case its in use
  digitalWrite(GPSPOWER,LOW);
  
  GPSserial.begin(GPSBaud);                  //start softserial for GPS at defined baud rate
  led_Flash(5,100);                          //series of fast LED flshes as test  
     
}
