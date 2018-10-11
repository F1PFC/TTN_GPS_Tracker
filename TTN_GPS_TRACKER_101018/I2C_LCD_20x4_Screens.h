//I2C_20x4_LCD_Screens.h
/*
**************************************************************************************************

  Easy Build LoRaTracker Programs for Arduino

  Copyright of the author Stuart Robinson - 10/10/2018

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
  of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.

  To Do:

**************************************************************************************************
*/


void writescreen_1();             //tracker GPS location screen
void writescreen_2();             //waiting for GPS fix screen
void writescreen_3();             //have new GPS fix screen
void writescreen_4();             //EV_TXCOMPLETE screen
void writescreen_5();             //Welcome screen
//void writescreen_Alert1();        //No GPS fix warning

//void writeSNR(byte col, byte row);
//void revert_Screen();
void add_Tracker_Location();
//void add_Tracker_DD();
//void Display_Status();

void writePktCount(byte col, byte row, unsigned int packetcount);


/******************************************************
  Screens
*******************************************************/


void writescreen_1()
{
  Display_Clear();
  add_Tracker_Location();
  Display_SetCurPos(0, 2);
  disp.print(F("Hdop "));
  disp.print(TRhdopGPS,2);
  writePktCount(0,3,lora_TXpacketCount);
}


void writescreen_2()
{
  Display_Clear();
  Display_SetCurPos(0, 0);
  disp.print(F("Waiting for GPS fix"));
}


void writescreen_3()
{
  Display_Clear();
  Display_SetCurPos(0, 0);
  disp.print(F("New GPS fix"));
}


void writescreen_4()
{
  Display_Clear();
  Display_SetCurPos(0, 0);
  disp.print(F("EV_TXCOMPLETE"));
}


void writescreen_5()
{
  Display_Clear();
  Display_SetCurPos(0, 0);
  disp.print(F(programname));
  Display_SetCurPos(0, 1);
  disp.print(F(dateproduced));
  Display_SetCurPos(0, 2);
  disp.print(F("www.LoRaTracker.uk"));
}



void add_Tracker_Location()
{
  //prints the tracker lat, long and alt
  Display_SetCurPos(0, 0);
  disp.print(F("LAT "));
  disp.print(TRLat, 5);    //5 decimal places is enough, gives location to the nearest approx 10M
  Display_SetCurPos(0, 1);
  disp.print(F("LON "));
  disp.print(TRLon, 5);
  Display_SetCurPos(14, 0);
  disp.print(TRAlt,0);
  disp.print("M ");
}



void writePktCount(byte col, byte row, unsigned int packetcount)
{
    Display_SetCurPos(col, row);                   //cursor to last position
    disp.print(F("Sent "));
    disp.print(packetcount);                       //send count to LCD
}

/*
void writescreen_Alert1()
{
  //Display_SetTextSize(1);
  Display_Clear();                                    //set large font and clear
  Display_SetCurPos(0, 2);
  disp.print(F("No GPS Fix"));
  delay(1500);
  //revert_Screen();
}
*/
/*
void revert_Screen()
{
  if (current_screen_number == 1)
  {
    writescreen_1();
  }

  if (current_screen_number == 2)
  {
    writescreen_2();
  }
  delay(1000);   //leave screen up for a while
}
*/
