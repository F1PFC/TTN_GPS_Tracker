//All_Configuration.h
//configuration file for TTN_GPS_Tracker

//**************************************************************************************************
// 1) Hardware related definitions and options - specify board definition file type here
//**************************************************************************************************

#define Board_Definition "HAB3_Board_Definitions.h"                        //if your using a LoRaTracker HAB3 Board enable this define 
//#define Board_Definition "LCD_Receiver_Board_Definitions.h"              //if your using a LoRaTracker LCD Receiver enable this define
//#define Board_Definition "Pro_Mini_Mikrobus_Shield_Board_Definitions.h"  //if your using a LoRaTracker Pro Mini Mikrobus Shield enable this define
//#define Board_Definition "Locator2_Board_Definitions.h"                  //if your using a LoRaTracker Locator2 Board enable this define
//#define Board_Definition "Custom_Board_Definitions.h"                    //if your using a custom board enable this define and setup the Custom_Board_Definitions.h file

#define LoRa_Device_in_MB1                                                  //needed for Mikrobus based boards
#define GPS_in_MB2                                                          //needed for Mikrobus based boards

//**************************************************************************************************
// 2) Program Options - Keys for LoRaWAN etc
//**************************************************************************************************

//LoRaWAN NwkSKey, network session key
static const PROGMEM u1_t NWKSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 

//LoRaWAN AppSKey, application session key
static const u1_t PROGMEM APPSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 

//LoRaWAN end-device address (DevAddr) Change this address for every node!
static const u4_t DEVADDR = 0x00000000;

const unsigned TX_INTERVAL_SECONDS = 55;        //approximate time between transmissions in seconds, actual time around 5 seconds greater


//**************************************************************************************************
// 3) GPS Options
//**************************************************************************************************

static const uint16_t GPSBaud = 9600;            //GPS baud rate

//#define Use_Test_Location                      //to use a test location for transmissions include this define

//Pen-y-Fan summit, Brecon Beacons
#define TestLatitude 51.88406
#define TestLongitude -3.43644
#define TestAltitude 886


//**************************************************************************************************
// 4) Display Settings - Need to define a library to use and screens
//**************************************************************************************************

//#define Use_Display                                      //if your using a display set this option   
#define Display_Library "Display_I2C_LCD2.h"               //This is the 20x4 LCD display, driven by a PCF8574 backpack 
#define Display_Screens "I2C_LCD_20x4_Screens.h"           //use the I2C LCD Screens, 20x4 
const int PCF8574Address = 0x27;                           //I2C address of the PCF8574, this define allows the address to be specified, normally 0x27 or 0x3F

//#define Display_Library "Display_SD1306_AVR.h"            //use an SSD1306 I2C display 
//#define Display_Screens "SD1306_SMALL_TEXT_Screens.h"     //use small text on display, more information   
const int I2C_ADDRESS = 0x3C;                               //address of SD13206 will be 0x3C or 0x3D

const unsigned int screen_delaymS = 2000;                //time in mS a temporary screen is displayed 
