//TTN_GPS_TRACKER_CayenneLPP_121018.ino

//**************************************************************************************************
// Important Note:
//
// Make changes to this Program file at your peril
//
// Configuration changes should be made in the configuration.h file not here !
//
// See seperate document 'Using the All_Configuration File.pdf' for information on setting up the 
// All_Configuration.h file. 
//
//**************************************************************************************************


/*******************************************************************
  Original code by Marco Rudolpf published in this article;

  https://www.thethingsnetwork.org/labs/story/lorawan-gsp-tracker

  Adapted by Stuart Robinson 10th October 2018.

  Changes:
  Switch to NeoSWSerial library for reading GPS
  Change software serial instance from ss to GPSserial
  Change pin mapping to accept LoRaTracker board definition files.
  Add LED1 definition and flashes for watchdog
  Changed payload format to match default in TTN console
  Added GPSwaitfix before program starts.
  Remove print of GPS output to monitor
  Change get_coords() to always wait for new GPS fix
  Add option to attache a I2C LCD display via a PCF8274 backpack
  Add option to attach a SSD1306 OLED display 

*********************************************************************/

#define programname "TTN_GPS_TRACKER"
#define dateproduced "101018"
#define aurthorname "Stuart Robinson"

#include <lmic.h>                                 //https://github.com/matthijskooijman/arduino-lmic 
#include <hal/hal.h>                              //https://github.com/matthijskooijman/arduino-lmic

#include "All_Configuration.h"

#include <SPI.h>                                  //standard Arduino library
#include <TinyGPS++.h>                            //https://github.com/mikalhart/TinyGPSPlus

#define LPP_GPS 136

#include Board_Definition                         //include previously defined board definition file

float TRLat, TRLon, TRAlt, TRhdopGPS, gps_increment;
unsigned int lora_TXpacketCount;


#ifdef Use_Display
#include Display_Library                          //include previously defined Display Library
#include Display_Screens                          //include previously defined screens 
#endif

TinyGPSPlus gps;                                  //define GPS instance

#include <NeoSWSerial.h>                          //https://github.com/SlashDevin/NeoSWSerial  
NeoSWSerial GPSserial(GPSRX, GPSTX);              //this library is more relaible at GPS init than software serial


void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }


uint8_t coords[11];
static osjob_t sendjob;
static osjob_t initjob;
uint8_t cursor = 0;
uint8_t channel;


//Pin mapping
const lmic_pinmap lmic_pins = {
  .nss = lora_NSS,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = LMIC_UNUSED_PIN,
  .dio = {lora_DIO0, lora_DIO1, lora_DIO2},
};



void loop()
{
  os_runloop_once();
}


boolean gpsWaitFix()
{
  Watchdog_Pulse();                                  //pulse LED in case we have a watchdog connected

  while (GPSserial.available() > 0)
    gps.encode(GPSserial.read());

  if (gps.location.isUpdated() && gps.altitude.isUpdated())
  {
    return true;
  }
  else
  {
    return false;
  }
}


void get_coords ()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  float fhdopGPS;
  unsigned long age;

  digitalWrite(LED1, HIGH);                            //LED on to indicate start of GPS check

#ifdef Use_Display
  writescreen_2();
#endif


  Serial.println(F("Waiting for GPS Fix"));
  while (!gpsWaitFix());                               //wait for another GPS fix
  digitalWrite(LED1, LOW);                             //finished with GPS, turn LED off
  Serial.println(F("Have GPS Fix"));


#ifdef Use_Display
  writescreen_3();
#endif


#ifdef Use_Test_Location
  Serial.println(F("Use GPS Test Location"));
  TRLat = TRLat + gps_increment;
  TRLon = TRLon + gps_increment;
  TRAlt = TRAlt + 10;
  TRhdopGPS = 100;
  TRhdopGPS = TRhdopGPS / 100;
  fhdopGPS = gps.hdop.value();
  gps_increment = gps_increment + 0.02;
#endif


#ifndef Use_Test_Location
  TRLat = gps.location.lat();
  TRLon = gps.location.lng();
  if (gps.altitude.isValid())
  {
    TRAlt = gps.altitude.meters();
  }
  else
  {
    TRAlt = 0;
  }
  //TRhdopGPS = (float) (gps.hdop.value() / 100);
  TRhdopGPS = gps.hdop.value();
  TRhdopGPS = TRhdopGPS / 100;
  fhdopGPS = gps.hdop.value();
#endif


  int32_t lat = TRLat * 10000;
  int32_t lon = TRLon * 10000;
  int16_t altitudeGPS = TRAlt * 100;
  int8_t hdopGPS = fhdopGPS;


channel = 0x01;
  coords[0] = channel;
  coords[1] = LPP_GPS;
  coords[2] = lat >> 16;
  coords[3] = lat >> 8;
  coords[4] = lat;
  coords[5] = lon >> 16;
  coords[6] = lon >> 8;
  coords[7] = lon;
  coords[8] = altitudeGPS;
  coords[9] = altitudeGPS >> 8;
  coords[10] = hdopGPS;



/*
  coords[0] = lat >> 16;
  coords[1] = lat >> 8;
  coords[2] = lat;
  coords[3] = lon >> 16;
  coords[4] = lon >> 8;
  coords[5] = lon;
  coords[6] = altitudeGPS >> 8;
  coords[7] = altitudeGPS;
  coords[8] = hdopGPS;
*/

  channel = 0x01;
  coords[0] = channel;
  coords[1] = LPP_GPS;
  coords[2] = lat >> 16;
  coords[3] = lat >> 8;
  coords[4] = lat;
  coords[5] = lon >> 16;
  coords[6] = lon >> 8;
  coords[7] = lon;
  coords[8] = altitudeGPS;
  coords[9] = altitudeGPS >> 8;
  coords[10] = hdopGPS;


#ifdef Use_Display
  writescreen_1();
#endif

}


void do_send(osjob_t* j)
{
  //Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND)
  {
    Serial.println(F("OP_TXRXPEND, not sending"));
  }
  else
  {
    //Prepare upstream data transmission at the next possible time.
    get_coords();
    LMIC_setTxData2(1, (uint8_t*) coords, sizeof(coords), 0);
    Serial.println(F("Packet queued"));
  }
}


void onEvent (ev_t ev)
{
  Serial.print(os_getTime());
  Serial.print(": ");
  switch (ev) {
    case EV_SCAN_TIMEOUT:
      Serial.println(F("EV_SCAN_TIMEOUT"));
      break;

    case EV_BEACON_FOUND:
      Serial.println(F("EV_BEACON_FOUND"));
      break;

    case EV_BEACON_MISSED:
      Serial.println(F("EV_BEACON_MISSED"));
      break;

    case EV_BEACON_TRACKED:
      Serial.println(F("EV_BEACON_TRACKED"));
      break;

    case EV_JOINING:
      Serial.println(F("EV_JOINING"));
      break;

    case EV_JOINED:
      Serial.println(F("EV_JOINED"));
      // Disable link check validation (automatically enabled during join, but not supported by TTN at this time).
      LMIC_setLinkCheckMode(0);
      break;

    case EV_RFU1:
      Serial.println(F("EV_RFU1"));
      break;

    case EV_JOIN_FAILED:
      Serial.println(F("EV_JOIN_FAILED"));
      break;

    case EV_REJOIN_FAILED:
      Serial.println(F("EV_REJOIN_FAILED"));
      break;

    case EV_TXCOMPLETE:
      Serial.println(F("EV_TXCOMPLETE"));
      lora_TXpacketCount++;
      
      #ifdef Use_Display
      writescreen_4();                          //write screen showing transmission complete
      delay(screen_delaymS);                    //leave screen visiuuble for a while    
      writescreen_1();                          //put GPS location screen back up 
      #endif
      
      
      if (LMIC.txrxFlags & TXRX_ACK) Serial.println(F("Received ack"));
      if (LMIC.dataLen)
      {
        Serial.println(F("Received "));
        Serial.println(LMIC.dataLen);
        Serial.println(F(" bytes of payload"));
      }

      //Schedule next transmission
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL_SECONDS), do_send);
      break;

    case EV_LOST_TSYNC:
      Serial.println(F("EV_LOST_TSYNC"));
      break;

    case EV_RESET:
      Serial.println(F("EV_RESET"));
      break;

    case EV_RXCOMPLETE:
      //data received in ping slot
      Serial.println(F("EV_RXCOMPLETE"));
      break;

    case EV_LINK_DEAD:
      Serial.println(F("EV_LINK_DEAD"));
      break;

    case EV_LINK_ALIVE:
      Serial.println(F("EV_LINK_ALIVE"));
      break;

    default:
      Serial.println(F("Unknown event"));
      break;
  }
}


void Watchdog_Pulse()
{
  digitalWrite(LED1, !digitalRead(LED1));       //toggle LED
  digitalWrite(LED1, !digitalRead(LED1));
}


void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(F(programname));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();
  
  GPSserial.begin(GPSBaud);

  pinMode(LED1, OUTPUT);
  Watchdog_Pulse();                              //toggle watchdog pin

  pinMode(GPSPOWER, OUTPUT);
  digitalWrite(GPSPOWER, LOW);

#ifdef Use_Display
  Display_Setup();
  writescreen_5();
  delay(screen_delaymS);
  writescreen_2();                               //write intial messager to display
#endif

  Serial.println(F("Waiting for Startup GPS Fix"));
  digitalWrite(LED1, HIGH);                      //LED on to indicate GPS check
  while (!gpsWaitFix());                         //wait for intial GPS fix
  digitalWrite(LED1, LOW);


  #ifdef Use_Test_Location
  TRLat = TestLatitude;
  TRLon = TestLongitude;
  TRAlt = TestAltitude;
#endif


  os_init();                                     //LMIC init
  LMIC_reset();                                  //Reset the MAC state. Session and pending data transfers will be discarded.

  //Set static session parameters. Instead of dynamically establishing a session
  //by joining the network, precomputed session parameters are be provided.

#ifdef PROGMEM
  //On AVR, these values are stored in flash and only copied to RAM
  //once. Copy them to a temporary buffer here, LMIC_setSession will
  //copy them into a buffer of its own again.

  uint8_t appskey[sizeof(APPSKEY)];
  uint8_t nwkskey[sizeof(NWKSKEY)];
  memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
  memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
  LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
#else
  // If not running an AVR with PROGMEM, just use the arrays directly
  LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
#endif


#if defined(CFG_eu868)
  // Set up the channels used by the Things Network, which corresponds
  // to the defaults of most gateways. Without this, only three base
  // channels from the LoRaWAN specification are used, which certainly
  // works, so it is good for debugging, but can overload those
  // frequencies, so be sure to configure the full frequency range of
  // your network here (unless your network autoconfigures them).
  // Setting up channels should happen after LMIC_setSession, as that
  // configures the minimal channel set.
  // NA-US channels 0-71 are configured automatically

  LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
  LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band

  // LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  // LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  // LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  // LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  // LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  // LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
  // TTN defines an additional channel at 869.525Mhz using SF9 for class B
  // devices' ping slots. LMIC does not have an easy way to define set this
  // frequency and support for class B is spotty and untested, so this
  // frequency is not configured here.

#elif defined(CFG_us915)

  // NA-US channels 0-71 are configured automatically
  // but only one group of 8 should (a subband) should be active
  // TTN recommends the second sub band, 1 in a zero based count.
  // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json

  LMIC_selectSubBand(1);

#endif

  LMIC_setLinkCheckMode(0);              //Disable link check validation
  LMIC.dn2Dr = DR_SF9;                   //TTN uses SF9 for its RX2 window.
  LMIC_setDrTxpow(DR_SF7, 14);           //Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
  do_send(&sendjob);                     //Start job

  Watchdog_Pulse();
}
