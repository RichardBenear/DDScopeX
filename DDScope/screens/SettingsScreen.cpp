// =====================================================
// SettingsScreen.cpp

// Author: Richard Benear 2/13/22

#include "SettingsScreen.h"
#include "../catalog/Catalog.h"
#include "../fonts/Inconsolata_Bold8pt7b.h"
#include "src/telescope/mount/Mount.h"
#include "../../../lib/tasks/OnTask.h"

#define PAD_BUTTON_X         2
#define PAD_BUTTON_Y         280
#define PAD_BUTTON_W         50
#define PAD_BUTTON_H         40
#define PAD_BUTTON_SPACING_X 1
#define PAD_BUTTON_SPACING_Y 1

#define TXT_LABEL_X         5
#define TXT_LABEL_Y         181
#define TXT_FIELD_X         135
#define TXT_FIELD_Y         TXT_LABEL_Y+2
#define TXT_FIELD_WIDTH     65
#define TXT_FIELD_HEIGHT    16
#define TXT_SPACING_X       10
#define TXT_SPACING_Y       23

#define T_SELECT_X           205
#define T_SELECT_Y           167
#define T_CLEAR_X            265
#define T_CLEAR_Y            T_SELECT_Y
#define CO_BOXSIZE_X         50
#define CO_BOXSIZE_Y         23

#define D_SELECT_X           T_SELECT_X
#define D_SELECT_Y           T_SELECT_Y+CO_BOXSIZE_Y
#define D_CLEAR_X            T_CLEAR_X
#define D_CLEAR_Y            D_SELECT_Y

#define U_SELECT_X           T_SELECT_X
#define U_SELECT_Y           T_SELECT_Y+2*CO_BOXSIZE_Y
#define U_CLEAR_X            T_CLEAR_X
#define U_CLEAR_Y            U_SELECT_Y

#define LA_SELECT_X          T_SELECT_X
#define LA_SELECT_Y          T_SELECT_Y+3*CO_BOXSIZE_Y
#define LA_CLEAR_X           T_CLEAR_X
#define LA_CLEAR_Y           LA_SELECT_Y

#define LO_SELECT_X          T_SELECT_X
#define LO_SELECT_Y          T_SELECT_Y+4*CO_BOXSIZE_Y
#define LO_CLEAR_X           T_CLEAR_X
#define LO_CLEAR_Y           LO_SELECT_Y

#define S_SEND_BUTTON_X      217
#define S_SEND_BUTTON_Y      282
#define S_SEND_BOXSIZE_X     80
#define S_SEND_BOXSIZE_Y     25

#define SITE_BUTTON_X        210
#define SITE_BUTTON_Y        321
#define SITE_BOXSIZE_X       90
#define SITE_BOXSIZE_Y       20

#define TDU_DISP_X           156
#define TDU_DISP_Y           355
#define TDU_OFFSET_X         80
#define TDU_OFFSET_Y         20

#define CUSTOM_FONT_OFFSET  -TXT_FIELD_HEIGHT+2
#define CHAR_WIDTH           8

// Settings Screen Button object
Button settingsButton(
                0, 0, 0, 0,
                butOnBackground, 
                butBackground, 
                butOutline, 
                mainFontWidth, 
                mainFontHeight, 
                "");
                
// Canvas Print object Custom Font
CanvasPrint canvSettingsInsPrint(&Inconsolata_Bold8pt7b);

// ===== Draw the SETTINGS Page =====
void SettingsScreen::draw() {
  setCurrentScreen(SETTINGS_SCREEN);
  tft.setTextColor(textColor);
  tft.fillScreen(pgBackground);
  
  drawTitle(100, TITLE_TEXT_Y, "Settings");
  drawMenuButtons();
  tft.setFont(&Inconsolata_Bold8pt7b);
  drawCommonStatusLabels(); // status common to many pages
  updateCommonStatus();
  updateSettingsButtons(false);
  getOnStepCmdErr(); // show error bar

  TtextIndex = 0; 
  DtextIndex = 0; 
  TztextIndex = 0; 
  LaTextIndex = 0; 
  LoTextIndex = 0; 

  // Draw Entry Field Labels
  tft.setCursor(TXT_LABEL_X, TXT_LABEL_Y);
  tft.print("Time 24h(hhmmss):");
  tft.setCursor(TXT_LABEL_X, TXT_LABEL_Y+TXT_SPACING_Y);
  tft.print(" Date (mm/dd/yy):");
  tft.setCursor(TXT_LABEL_X, TXT_LABEL_Y+TXT_SPACING_Y*2);
  tft.print(" Time Zone (sHH):");
  tft.setCursor(TXT_LABEL_X, TXT_LABEL_Y+TXT_SPACING_Y*3);
  tft.print("Latitude (sXX.X):");
  tft.setCursor(TXT_LABEL_X, TXT_LABEL_Y+TXT_SPACING_Y*4);
  tft.print("Longitud(sXXX.X):");

  tft.setCursor(TDU_DISP_X, TDU_DISP_Y);
  tft.print("     Time:");
  tft.setCursor(TDU_DISP_X, TDU_DISP_Y+TDU_OFFSET_Y);
  tft.print("     Date:");
  tft.setCursor(TDU_DISP_X, TDU_DISP_Y+TDU_OFFSET_Y*2);
  tft.print("       TZ:");
  tft.setCursor(TDU_DISP_X, TDU_DISP_Y+TDU_OFFSET_Y*3);
  tft.print(" Latitude:");
  tft.setCursor(TDU_DISP_X, TDU_DISP_Y+TDU_OFFSET_Y*4);
  tft.print("Longitude:");

  // Draw Key Pad
  int z=0;
  for(int i=0; i<4; i++) { 
    for(int j=0; j<3; j++) {
      int row=i; int col=j; 
      settingsButton.draw(PAD_BUTTON_X+col*(PAD_BUTTON_W+PAD_BUTTON_SPACING_X), 
                          PAD_BUTTON_Y+row*(PAD_BUTTON_H+PAD_BUTTON_SPACING_Y), 
                          PAD_BUTTON_W, PAD_BUTTON_H, sNumLabels[z], BUT_OFF);
      z++;
    }
  }

  // Initialize the background for the Text Entry fields
  tft.fillRect(TXT_FIELD_X, TXT_FIELD_Y+                CUSTOM_FONT_OFFSET, TXT_FIELD_WIDTH, TXT_FIELD_HEIGHT, butBackground);
  tft.fillRect(TXT_FIELD_X, TXT_FIELD_Y+TXT_SPACING_Y+  CUSTOM_FONT_OFFSET, TXT_FIELD_WIDTH, TXT_FIELD_HEIGHT, butBackground);
  tft.fillRect(TXT_FIELD_X, TXT_FIELD_Y+TXT_SPACING_Y*2+CUSTOM_FONT_OFFSET, TXT_FIELD_WIDTH, TXT_FIELD_HEIGHT, butBackground);
  tft.fillRect(TXT_FIELD_X, TXT_FIELD_Y+TXT_SPACING_Y*3+CUSTOM_FONT_OFFSET, TXT_FIELD_WIDTH, TXT_FIELD_HEIGHT, butBackground);
  tft.fillRect(TXT_FIELD_X, TXT_FIELD_Y+TXT_SPACING_Y*4+CUSTOM_FONT_OFFSET, TXT_FIELD_WIDTH, TXT_FIELD_HEIGHT, butBackground);
} // end initialize

// task update for this screen
void SettingsScreen::updateSettingsStatus() {
  // Get and show the Time and Location status
  updateCommonStatus();

  char tempReply[10];
  // show Local Time 24 Hr format
  getLocalCmdTrim(":GL#", tempReply); 
  canvSettingsInsPrint.printRJ(TDU_DISP_X+TDU_OFFSET_X, TDU_DISP_Y, 80, 16, tempReply, false);

  // show Current Date
  getLocalCmdTrim(":GC#", tempReply); 
  canvSettingsInsPrint.printRJ(TDU_DISP_X+TDU_OFFSET_X, TDU_DISP_Y+TDU_OFFSET_Y, 80, 16, tempReply, false);

  // show TZ Offset
  getLocalCmdTrim(":GG#", tempReply); 
  canvSettingsInsPrint.printRJ(TDU_DISP_X+TDU_OFFSET_X, TDU_DISP_Y+TDU_OFFSET_Y*2, 80, 16, tempReply, false);

  // show Latitude
  getLocalCmdTrim(":Gt#", tempReply); 
  canvSettingsInsPrint.printRJ(TDU_DISP_X+TDU_OFFSET_X, TDU_DISP_Y+TDU_OFFSET_Y*3, 80, 16, tempReply, false);

  // show Longitude
  getLocalCmdTrim(":Gg#", tempReply); 
  canvSettingsInsPrint.printRJ(TDU_DISP_X+TDU_OFFSET_X, TDU_DISP_Y+TDU_OFFSET_Y*4, 80, 16, tempReply, false);
}

// Numpad handler, assign label to pressed button field array slot
void SettingsScreen::setProcessNumPadButton() {
  if (sNumDetected) {
    // Time Format: [HH:MM:SS]
    if (Tselect && TtextIndex < 8 && (sButtonPosition != 9 || sButtonPosition != 11)) {
      Ttext[TtextIndex] = sNumLabels[sButtonPosition][0];
      tft.setCursor(TXT_FIELD_X+TtextIndex*CHAR_WIDTH, TXT_FIELD_Y);
      tft.print(Ttext[TtextIndex]); 
      TtextIndex++;
      if (TtextIndex == 2 || TtextIndex == 5) {
        tft.setCursor(TXT_FIELD_X+TtextIndex*CHAR_WIDTH, TXT_FIELD_Y);
        tft.print(':'); 
        TtextIndex++;
      }
    }

    // Date Format: [DD/MM/YY] last two digits of year
    if (Dselect && DtextIndex < 8 && (sButtonPosition != 9 || sButtonPosition != 11)) {
      Dtext[DtextIndex] = sNumLabels[sButtonPosition][0]; 
      tft.setCursor(TXT_FIELD_X+DtextIndex*CHAR_WIDTH, TXT_FIELD_Y+TXT_SPACING_Y);
      tft.print(Dtext[DtextIndex]); 
      DtextIndex++;
      if (DtextIndex == 2 || DtextIndex == 5) {
        tft.setCursor(TXT_FIELD_X+DtextIndex*CHAR_WIDTH, TXT_FIELD_Y+TXT_SPACING_Y);
        tft.print('/'); 
        DtextIndex++;
      }
    }

    // Time Zone Format: [sDD] first character must be a +/- and allow only 2 additional chars
    if (Tzselect && (((TztextIndex == 0 && (sButtonPosition == 9 || sButtonPosition == 11)) 
          || (TztextIndex>0 && (sButtonPosition!=9||sButtonPosition!=11)))) && TztextIndex < 3) { 
      Tztext[TztextIndex] = sNumLabels[sButtonPosition][0]; 
      tft.setCursor(TXT_FIELD_X+TztextIndex*CHAR_WIDTH, TXT_FIELD_Y+(TXT_SPACING_Y*2));
      tft.print(Tztext[TztextIndex]);
      TztextIndex++;
    }

    //Latitude Format: [sDD.D] first character must be a +/- and require 3 additional chars. Automatically put decimal in after first 2
    if (LaSelect && (((LaTextIndex == 0 && (sButtonPosition == 9 || sButtonPosition == 11)) 
        || (LaTextIndex>0 && (sButtonPosition!=9||sButtonPosition!=11)))) && LaTextIndex < 5) { //
      LaText[LaTextIndex] = sNumLabels[sButtonPosition][0]; 

      tft.setCursor(TXT_FIELD_X+LaTextIndex*CHAR_WIDTH, TXT_FIELD_Y+(TXT_SPACING_Y*3));
      tft.print(LaText[LaTextIndex]);
      LaTextIndex++;
      if (LaSelect && LaTextIndex == 3) {
        tft.setCursor(TXT_FIELD_X+LaTextIndex*CHAR_WIDTH, TXT_FIELD_Y+(TXT_SPACING_Y*3));
        tft.print(".");
        LaTextIndex++;
      }
    }

    //Longitude Format: [sDDD.D] first character must be a +/- and require 4 additional chars. Automatically put decimal in after first 3
    if (LoSelect && (((LoTextIndex == 0 && (sButtonPosition == 9 || sButtonPosition == 11)) 
      || (LoTextIndex>0 && (sButtonPosition!=9||sButtonPosition!=11)))) && LoTextIndex < 6) {
      LoText[LoTextIndex] = sNumLabels[sButtonPosition][0]; 
      
      tft.setCursor(TXT_FIELD_X+LoTextIndex*CHAR_WIDTH, TXT_FIELD_Y+(TXT_SPACING_Y*4));
      tft.print(LoText[LoTextIndex]);
      LoTextIndex++;
      if (LoSelect && LoTextIndex == 4) {
        tft.setCursor(TXT_FIELD_X+LoTextIndex*CHAR_WIDTH, TXT_FIELD_Y+(TXT_SPACING_Y*4));
        tft.print(".");
        LoTextIndex++;
      }
    }
    sNumDetected = false;
  }
}

bool SettingsScreen::settingsButStateChange() {
  if (display._redrawBut) {
    display._redrawBut = false;
    return true;
  } else { 
    return false;
  }
}

// Update the buttons for the Settings Screen
void SettingsScreen::updateSettingsButtons(bool redrawBut) {
  _redrawBut = redrawBut;
  // Get button and print label
  switch (sButtonPosition) {
    case 0:  setProcessNumPadButton(); break;
    case 1:  setProcessNumPadButton(); break;
    case 2:  setProcessNumPadButton(); break;
    case 3:  setProcessNumPadButton(); break;
    case 4:  setProcessNumPadButton(); break;
    case 5:  setProcessNumPadButton(); break;
    case 6:  setProcessNumPadButton(); break;
    case 7:  setProcessNumPadButton(); break;
    case 8:  setProcessNumPadButton(); break;
    case 9:  setProcessNumPadButton(); break;
    case 10: setProcessNumPadButton(); break;
    case 11: setProcessNumPadButton(); break;
    default: break;
  }

  // Time Select Button
  if (Tselect) {
    settingsButton.draw(T_SELECT_X, T_SELECT_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "TiSel", BUT_ON);
  } else {
    settingsButton.draw(T_SELECT_X, T_SELECT_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "TiSel", BUT_OFF);
  }

  // Time Clear button
  if (Tclear) {
    settingsButton.draw(T_CLEAR_X, T_CLEAR_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "TiClr", BUT_ON);
    tft.fillRect(TXT_FIELD_X, TXT_FIELD_Y+CUSTOM_FONT_OFFSET, TXT_FIELD_WIDTH, TXT_FIELD_HEIGHT, butBackground);
    memset(Ttext,0,sizeof(Ttext)); // clear Time buffer
    TtextIndex = 0;
    sButtonPosition = 12;
    Tclear = false;
  } else {
    settingsButton.draw(T_CLEAR_X, T_CLEAR_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "TiClr", BUT_OFF);
  }
  
  // Date Select button
  if (Dselect) {
    settingsButton.draw(D_SELECT_X, D_SELECT_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "DaSel", BUT_ON);
  } else {
    settingsButton.draw(D_SELECT_X, D_SELECT_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "DaSel", BUT_OFF); 
  }

  // Date Clear Button
  if (Dclear) {
    settingsButton.draw( D_CLEAR_X, D_CLEAR_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "DaClr", BUT_ON); 
    tft.fillRect(TXT_FIELD_X, TXT_FIELD_Y+CUSTOM_FONT_OFFSET+TXT_SPACING_Y, TXT_FIELD_WIDTH, TXT_FIELD_HEIGHT, butBackground);
    memset(Dtext,0,sizeof(Dtext)); // clear DATE buffer
    DtextIndex = 0;
    sButtonPosition = 12;
    Dclear = false;
  } else {
    settingsButton.draw(D_CLEAR_X, D_CLEAR_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "DaClr", BUT_OFF); 
  }

  // Time Zone Select button
  if (Tzselect) {
    settingsButton.draw(U_SELECT_X, U_SELECT_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "TzSel", BUT_ON);
  } else {
    settingsButton.draw(U_SELECT_X, U_SELECT_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "TzSel", BUT_OFF); 
  }

  // Time Zone Clear Button
  if (Tzclear) {
    settingsButton.draw(U_CLEAR_X, U_CLEAR_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "TzClr", BUT_ON); 
    tft.fillRect(TXT_FIELD_X, TXT_FIELD_Y+CUSTOM_FONT_OFFSET+TXT_SPACING_Y*2, TXT_FIELD_WIDTH, TXT_FIELD_HEIGHT, butBackground);
    memset(Tztext,0,sizeof(Tztext)); // clear TZ buffer
    TztextIndex = 0;
    sButtonPosition = 12;
    Tzclear = false;
  } else {
    settingsButton.draw(U_CLEAR_X, U_CLEAR_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "TzClr", BUT_OFF); 
  }

  // Latitude Select button
  if (LaSelect) {
    settingsButton.draw(LA_SELECT_X, LA_SELECT_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "LaSel", BUT_ON);
  } else {
    settingsButton.draw(LA_SELECT_X, LA_SELECT_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "LaSel", BUT_OFF); 
  }

  // Latitude Clear Button
  if (LaClear) {
    settingsButton.draw(LA_CLEAR_X, LA_CLEAR_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "LaClr", BUT_ON); 
    tft.fillRect(TXT_FIELD_X, TXT_FIELD_Y+CUSTOM_FONT_OFFSET+TXT_SPACING_Y*3, TXT_FIELD_WIDTH, TXT_FIELD_HEIGHT, butBackground);
    memset(LaText,0,sizeof(LaText)); // clear latitude buffer
    LaTextIndex = 0;
    sButtonPosition = 12;
    LaClear = false;
  } else {
    settingsButton.draw(LA_CLEAR_X, LA_CLEAR_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "LaClr", BUT_OFF); 
  }

  // Longitude Select button
  if (LoSelect) {
    settingsButton.draw(LO_SELECT_X, LO_SELECT_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "LoSel", BUT_ON);
  } else {
    settingsButton.draw(LO_SELECT_X, LO_SELECT_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "LoSel", BUT_OFF); 
  }

  // Longitude Clear Button
  if (LoClear) {
    settingsButton.draw(LO_CLEAR_X, LO_CLEAR_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "LoClr", BUT_ON); 
    tft.fillRect(TXT_FIELD_X, TXT_FIELD_Y+CUSTOM_FONT_OFFSET+TXT_SPACING_Y*4, TXT_FIELD_WIDTH, TXT_FIELD_HEIGHT, butBackground);
    memset(LoText,0,sizeof(LoText)); // clear longitude buffer
    LoTextIndex = 0;
    sButtonPosition = 12;
    LoClear = false;
  } else {
    settingsButton.draw(LO_CLEAR_X, LO_CLEAR_Y, CO_BOXSIZE_X, CO_BOXSIZE_Y, "LoClr", BUT_OFF); 
  }
  
  // Send Data Button
  if (sSendOn) {
    settingsButton.draw(S_SEND_BUTTON_X, S_SEND_BUTTON_Y, S_SEND_BOXSIZE_X, S_SEND_BOXSIZE_Y, "Sent", BUT_ON);
    sSendOn = false; 
  } else {
    settingsButton.draw(S_SEND_BUTTON_X, S_SEND_BUTTON_Y, S_SEND_BOXSIZE_X, S_SEND_BOXSIZE_Y, "Send", BUT_OFF); 
  }
}

// **** TouchScreen was touched, determine which button *****
bool SettingsScreen::touchPoll(uint16_t px, uint16_t py) {
  //were number Pad buttons pressed?
  for(int i=0; i<4; i++) { 
    for(int j=0; j<3; j++) {
      int row=i; int col=j; 
      if (py >   PAD_BUTTON_Y+row*(PAD_BUTTON_H+PAD_BUTTON_SPACING_Y) 
       && py <  (PAD_BUTTON_Y+row*(PAD_BUTTON_H+PAD_BUTTON_SPACING_Y)) + PAD_BUTTON_H 
       && px >   PAD_BUTTON_X+col*(PAD_BUTTON_W+PAD_BUTTON_SPACING_X) 
       && px <  (PAD_BUTTON_X+col*(PAD_BUTTON_W+PAD_BUTTON_SPACING_X) + PAD_BUTTON_W)) {
        sButtonPosition=row*3+col;
        //VF("sButtonPosition="); VL(sButtonPosition);
        sNumDetected = true;
        BEEP;
        return true;
      }
    }
  }

  // Select Time field
  if (py > T_SELECT_Y && py < (T_SELECT_Y + CO_BOXSIZE_Y) && px > T_SELECT_X && px < (T_SELECT_X + CO_BOXSIZE_X)) {
    Tselect = true;
    Dselect = false;
    Tzselect = false;
    LaSelect = false;
    LoSelect = false;
    BEEP;
    return true;
  }

  // Clear Time field
  if (py > T_CLEAR_Y && py < (T_CLEAR_Y + CO_BOXSIZE_Y) && px > T_CLEAR_X && px < (T_CLEAR_X + CO_BOXSIZE_X)) {
    Tclear = true; 
    TtextIndex = 0;
    sButtonPosition = 12; 
    BEEP;
    return true;
  }

  // Select Date field
  if (py > D_SELECT_Y && py < (D_SELECT_Y + CO_BOXSIZE_Y) && px > D_SELECT_X && px < (D_SELECT_X + CO_BOXSIZE_X)) {
    Tselect = false;
    Dselect = true;
    Tzselect = false;
    LaSelect = false;
    LoSelect = false;
    BEEP;
    return true;
  }

  // Clear DEC field
  if (py > D_CLEAR_Y && py < (D_CLEAR_Y + CO_BOXSIZE_Y) && px > D_CLEAR_X && px < (D_CLEAR_X + CO_BOXSIZE_X)) {
    Dclear = true; 
    DtextIndex = 0;
    sButtonPosition = 12;
    BEEP;
    return true; 
  }

  // Select TZ field
  if (py > U_SELECT_Y && py < (U_SELECT_Y + CO_BOXSIZE_Y) && px > U_SELECT_X && px < (U_SELECT_X + CO_BOXSIZE_X)) {
    Tselect = false;
    Dselect = false;
    Tzselect = true;
    LaSelect = false;
    LoSelect = false;
    BEEP;
    return true;
  }

  // Clear TZ field
  if (py > U_CLEAR_Y && py < (U_CLEAR_Y + CO_BOXSIZE_Y) && px > U_CLEAR_X && px < (U_CLEAR_X + CO_BOXSIZE_X)) {
    Tzclear = true; 
    TztextIndex = 0;
    sButtonPosition = 12;
    BEEP;
    return true; 
  }

  // Select Latitude field
  if (py > LA_SELECT_Y && py < (LA_SELECT_Y + CO_BOXSIZE_Y) && px > LA_SELECT_X && px < (LA_SELECT_X + CO_BOXSIZE_X)) {
    Tselect = false;
    Dselect = false;
    Tzselect = false;
    LaSelect = true;
    LoSelect = false;
    BEEP;
    return true;
  }

  // Clear Latitude field
  if (py > LA_CLEAR_Y && py < (LA_CLEAR_Y + CO_BOXSIZE_Y) && px > LA_CLEAR_X && px < (LA_CLEAR_X + CO_BOXSIZE_X)) {
    LaClear = true; 
    LaTextIndex = 0;
    sButtonPosition = 12;
    BEEP;
    return true; 
  }

// Select Longitude field
  if (py > LO_SELECT_Y && py < (LO_SELECT_Y + CO_BOXSIZE_Y) && px > LO_SELECT_X && px < (LO_SELECT_X + CO_BOXSIZE_X)) {
    Tselect = false;
    Dselect = false;
    Tzselect = false;
    LaSelect = false;
    LoSelect = true;
    BEEP;
    return true;
  }

  // Clear Longitude field
  if (py > LO_CLEAR_Y && py < (LO_CLEAR_Y + CO_BOXSIZE_Y) && px > LO_CLEAR_X && px < (LO_CLEAR_X + CO_BOXSIZE_X)) {
    LoClear = true; 
    LoTextIndex = 0;
    sButtonPosition = 12;
    BEEP;
    return true; 
  }

  // SEND Data
  if (py > S_SEND_BUTTON_Y && py < (S_SEND_BUTTON_Y + S_SEND_BOXSIZE_Y) && px > S_SEND_BUTTON_X && px < (S_SEND_BUTTON_X + S_SEND_BOXSIZE_X)) {
    sSendOn = true; 
    TtextIndex  = 0;
    DtextIndex  = 0;
    TztextIndex  = 0;
    LaTextIndex = 0;
    LoTextIndex = 0;
    sButtonPosition = 12; 
    static char sLaMin[3] = "";
    static char sLoMin[3] = "";
    BEEP;
    
    if (Tselect) {
      // Set Local Time :SL[HH:MM:SS]# 24Hr format
      sprintf(sCmd, ":SL%c%c:%c%c:%c%c#", Ttext[0], Ttext[1], Ttext[2], Ttext[3], Ttext[4], Ttext[5]);
      setLocalCmd(sCmd);
      delay(70);
    } else if (Dselect) { // :SC[MM/DD/YY]# 
      sprintf(sCmd, ":SC%c%c/%c%c/%c%c#", Dtext[0], Dtext[1], Dtext[2], Dtext[3], Dtext[4], Dtext[5]);
      setLocalCmd(sCmd);
    } else if (Tzselect) { // :SG[sHH]# // Time Zone
      sprintf(sCmd, ":SG%c%c%c#", Tztext[0], Tztext[1], Tztext[2]);
      setLocalCmd(sCmd);
    } else if (LaSelect) { // :St[sDD*MM]#  Latitude
      uint8_t laMin = LaText[4] - '0';  //digit after decimal point
      sprintf(sLaMin, "%02d\n", laMin * 6); // convert fractional degrees to string minutes
      sprintf(sCmd, ":St%c%c%c*%2s#", LaText[0], LaText[1], LaText[2], sLaMin);
      setLocalCmd(sCmd);
    } else if (LoSelect) { // :Sg[(s)DDD*MM]#  Longitude
      uint8_t loMin = LoText[5] - '0'; //digit after decimal point
      sprintf(sLoMin, "%02d\n", loMin * 6); // convert fractional degrees to string minutes
      sprintf(sCmd, ":Sg%c%c%c%c*%2s#", LoText[0], LoText[1], LoText[2], LoText[3], sLoMin);
      setLocalCmd(sCmd);
    }
    return true;
  }
  return false;
}

SettingsScreen settingsScreen;
