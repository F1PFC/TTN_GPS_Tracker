//configuration file for TTN_GPS_Tracker

//**************************************************************************************************
// 1) Hardware related definitions and options - specify board definition file type here
//**************************************************************************************************

//#define Board_Definition "LCD_Receiver_Board_Definitions.h"  //define which board you are using here
#define Board_Definition "HAB3_Board_Definitions.h"

#define LoRa_Device_in_MB1                                   //needed for Mikrobus based boards
#define GPS_in_MB2                                           //needed for Mikrobus based boards

//**************************************************************************************************
// 2) Program Options - Keys for LoRaWAN etc
//**************************************************************************************************

//LoRaWAN NwkSKey, network session key
static const PROGMEM u1_t NWKSKEY[16] = { 0x61, 0x59, 0x09, 0x2C, 0xB4, 0x2D, 0x57, 0x4A, 0xEC, 0xCF, 0x1C, 0x0D, 0xF0, 0x92, 0x06, 0xF8 }; 

//LoRaWAN AppSKey, application session key
static const u1_t PROGMEM APPSKEY[16] = { 0x15, 0xEB, 0x81, 0x96, 0x1B, 0xE9, 0x91, 0xAC, 0x3F, 0x0C, 0x0F, 0x98, 0xDC, 0x49, 0x71, 0x98 }; 

//LoRaWAN end-device address (DevAddr) Change this address for every node!
static const u4_t DEVADDR = 0x26011C15 ; 

const unsigned TX_INTERVAL = 20;


//**************************************************************************************************
// 5) GPS Options
//**************************************************************************************************

static const uint32_t GPSBaud = 9600;                    //GPS baud rate


//**************************************************************************************************
// 7) Display Settings - Need to define a library to use and screens
//**************************************************************************************************

//#define Use_Display                                      //if your using a display set this option   
#define Display_Library "Display_I2C_LCD2.h"             //This is the 20x4 LCD display, driven by a PCF8574 backpack 
#define Display_Screens "I2C_LCD_20x4_Screens.h"         //use the I2C LCD Screens, 20x4 
const int PCF8574Address = 0x27;                         //I2C address of the PCF8574, this define allows the address to be specified, normally 0x27 or 0x3F
const unsigned int screen_delaymS = 2000;                //time in mS a temporary screen is displayed 
