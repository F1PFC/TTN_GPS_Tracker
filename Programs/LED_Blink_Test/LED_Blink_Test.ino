#define programname "LED_Blink_Test"
#define programversion "V1.0"
#define dateproduced "26/11/2017"
#define aurthorname "Stuart Robinson"
//#include <Arduino.h>

/*
*****************************************************************************************************************************
Tracker Test Programs

Copyright of the author Stuart Robinson - 26/11/2017



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

The program flashes a LED connected to the pin defined by LED1
********************************************************************************************************************************
*/

#define LED1 8                              //Arduino pin number for LED, when high LED should be on.

#define Serial_Monitor_Baud 115200          //this is baud rate used for the Arduino IDE Serial Monitor

unsigned int seconds;

void loop() 
{
  Serial.print(seconds);
  Serial.println(F(" Seconds"));            //this message should print on console close to once per second   
  seconds++;
  led_Flash(4,125);
}


void led_Flash(unsigned int flashes, unsigned int delaymS)
{
  //flash LED to show tracker is alive
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
  Serial.begin(Serial_Monitor_Baud);          //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();

  pinMode(LED1, OUTPUT);                     //for PCB LED
 }
