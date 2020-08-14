//LCD_Receiver_Board_Definitions.h

/*
*******************************************************************************************************************************
  Easy Build Tracker Programs for Arduino

  Copyright of the author Stuart Robinson - 10/10/18

  

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
  of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.

  This is the hardware definitions for a custome board.

  To Do:

*******************************************************************************************************************************
*/

//these pin definitions are mandatory
#define lora_NReset ?                //for a custom board setup define this pin
#define lora_NSS ?                   //for a custom board setup define this pin
#define lora_DIO0 ?                  //for a custom board setup define this pin
#define lora_DIO1 ?                  //for a custom board setup define this pin
#define lora_DIO2 ?                  //for a custom board setup define this pin
#define GPSTX ?                      //for a custom board setup define this pin
#define GPSRX ?                      //for a custom board setup define this pin
#define LED1 ?                       //pin number for LED


//these pin defginitions are optional

#define GPSPOWER UNUSED_PIN
#define SWITCH1 UNUSED_PIN
#define SupplyAD  UNUSED_PIN         //measure supply voltage here
#define BUZZ UNUSED_PIN

#define UNUSED_PIN -1

#define ADMultiplier 10              //multiplier for supply volts calculation
