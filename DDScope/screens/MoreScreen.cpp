// =====================================================
// MoreScreen.cpp
//
// CATALOG and "MORE" Menus and Feature
// The Catalog-and-More page allows access to More sub menus and Catalogs
// Author: Richard Benear - Dec 2021
//
// Uses the Catalog Manager portion from Smart Hand Controller (SHC).
// Copyright (C) 2018 to 2021 Charles Lemaire, Howard Dutton, and Others
// Author: Charles Lemaire, https://pixelstelescopes.wordpress.com/teenastro/
// Author: Howard Dutton, http://www.stellarjourney.com, hjd1964@gmail.com

#include "MoreScreen.h"
#include "TreasureCatScreen.h"
#include "CustomCatScreen.h"
#include "SHCCatScreen.h"
#include "PlanetsScreen.h"
#include "HomeScreen.h"
#include "../catalog/Catalog.h" // from SHC
#include "../fonts/Inconsolata_Bold8pt7b.h"
#include <Fonts/FreeSansBold9pt7b.h>
#include "../fonts/UbuntuMono_Bold11pt7b.h"
#include "../../../telescope/mount/Mount.h"

// Catalog Selection buttons
#define CAT_SEL_X               5
#define CAT_SEL_Y              179
#define CAT_SEL_BOXSIZE_X      110 
#define CAT_SEL_BOXSIZE_Y       28
#define CAT_SEL_SPACER          CAT_SEL_BOXSIZE_Y + 5

// Tracking rate buttons
#define TRACK_R_X              125
#define TRACK_R_Y              180
#define TRACK_R_BOXSIZE_X       70 
#define TRACK_R_BOXSIZE_Y       24
#define TRACK_R_SPACER           1 
#define TRACK_R_GROUP_SPACER     5 

// Misc Buttons
#define MISC_X                 212
#define MISC_Y                 164
#define MISC_BOXSIZE_X         100 
#define MISC_BOXSIZE_Y          28
#define MISC_SPACER_Y            8 

// Filter Button
#define FM_X                   200
#define FM_Y                   163
#define FM_BOXSIZE_X           120 
#define FM_BOXSIZE_Y            28
#define FM_SPACER_Y              2 

#define GOTO_BUT_X             212
#define GOTO_BUT_Y             264
#define GOTO_M_BOXSIZE_X       100
#define GOTO_M_BOXSIZE_Y        40

#define ABORT_M_BUT_X          212
#define ABORT_M_BUT_Y          310

const char *activeFilterStr[3] = {"Filt: None", "Filt: Abv Hor", "Filt: All Sky"};

// More Screen Button object with standard Font size
Button moreButton(TRACK_R_X, TRACK_R_Y, TRACK_R_BOXSIZE_X, TRACK_R_BOXSIZE_Y,
                butOnBackground, butBackground, butOutline, mainFontWidth, mainFontHeight, "");

// More Screen Button object with large Font size
Button moreLgButton(0, 0, 0, 0, butOnBackground, butBackground, butOutline, largeFontWidth, largeFontHeight, "");

// Canvas Print object, Inconsolata_Bold8pt7b font
CanvasPrint canvMoreInsPrint(&Inconsolata_Bold8pt7b);

// ============= Initialize the Catalog & More page ==================
void MoreScreen::draw() {
  setCurrentScreen(MORE_SCREEN);
  tft.setTextColor(textColor);
  tft.fillScreen(pgBackground);

  drawMenuButtons();
  drawTitle(70, TITLE_TEXT_Y, "Catalogs & Misc");

  // Draw the HOME Icon bitmap
  uint8_t extern black_house_icon[];
  tft.drawBitmap(10, 5, black_house_icon, 39, 31,  butBackground, ORANGE);

  // Draw the labels for columns
  tft.setFont(&Inconsolata_Bold8pt7b);  
  tft.setCursor(TRACK_R_X+12, TRACK_R_Y-7);
  tft.print("Rates");

  tft.setCursor(30, TRACK_R_Y-7);
  tft.print("Catalogs"); 

  drawCommonStatusLabels(); // Common status at top of most screens
  updateMoreButtons(false); // Draw initial More Page Buttons; false=no redraw
  getOnStepCmdErr(); // show error bar
}

// task update for this screen
void MoreScreen::updateMoreStatus() {
  updateCommonStatus();
}

bool MoreScreen::moreButStateChange() {
  if (preSlewState != mount.isSlewing()) {
    preSlewState = mount.isSlewing(); 
    return true;
   } else if (strcmp(preFilterState, activeFilterStr[activeFilter])) {
     strcpy(preFilterState, activeFilterStr[activeFilter]);
     return true;
  } else if (display._redrawBut) {
    display._redrawBut = false;
    return true;
  } else { 
    return false;
  }
}

//================== Update the Buttons ======================
void MoreScreen::updateMoreButtons(bool redrawBut) {
  _redrawBut = redrawBut;
    
  // Show any target object data that was selected from Catalog Screens
  uint16_t x = 120; uint16_t y = 358; 
  tft.fillRect(x-2, y+3, 199, 5*16, butBackground);
  
  tft.setCursor(x,y+16  ); tft.print(moreScreen.catSelectionStr1);
  tft.setCursor(x,y+16*2); tft.print(moreScreen.catSelectionStr2);
  tft.setCursor(x,y+16*3); tft.print(moreScreen.catSelectionStr3);
  tft.setCursor(x,y+16*4); tft.print(moreScreen.catSelectionStr4); 
  tft.setCursor(x,y+16*5); tft.print(moreScreen.catSelectionStr5); 

  int y_offset = 0;
  // Manage Tracking Rate buttons
  if (sidereal) { 
    moreButton.draw(TRACK_R_Y+y_offset, "Sidereal", BUT_ON);
    
    y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER;
    moreButton.draw(TRACK_R_Y+y_offset, "Lunar", BUT_OFF);

    y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER;
    moreButton.draw(TRACK_R_Y+y_offset, "King", BUT_OFF);
    
  }
  if (lunarRate) {
    y_offset = 0;
    moreButton.draw(TRACK_R_Y+y_offset, "Sidereal", BUT_OFF);

    y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER;
    moreButton.draw(TRACK_R_Y+y_offset, "Lunar", BUT_ON);
  
    y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER;
    moreButton.draw(TRACK_R_Y+y_offset, "King", BUT_OFF);
  }
  if (kingRate) {
    y_offset = 0;
    moreButton.draw(TRACK_R_Y+y_offset, "Sidereal", BUT_OFF);

    y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER;
    moreButton.draw(TRACK_R_Y+y_offset, "Lunar", BUT_OFF);

    y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER;
    moreButton.draw(TRACK_R_Y+y_offset, "King", BUT_ON);
  }

  // TO DO: figure out why incrementing or decrementing doesn't change the rate displayed below
  // increment tracking rate by 0.02 Hz
  y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER+TRACK_R_GROUP_SPACER ; // space between tracking setting fields
  if (incTrackRate) {
    moreButton.draw(TRACK_R_Y+y_offset, "IncTrack", BUT_ON);
    incTrackRate = false;
  } else {
    moreButton.draw(TRACK_R_Y+y_offset, "IncTrack", BUT_OFF);
  }   
  // decrement tracking rate by 0.02 Hz
  y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER; 
  if (decTrackRate) {
    moreButton.draw(TRACK_R_Y+y_offset, "DecTrack", BUT_ON);
    decTrackRate = false;
  } else {
    moreButton.draw(TRACK_R_Y+y_offset, "DecTrack", BUT_OFF);
  }   

  // Reset Tracking Rate Sidereal
  y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER+TRACK_R_GROUP_SPACER ; 
  if (rstTrackRate) {
    moreButton.draw(TRACK_R_Y+y_offset, "RstTrack", BUT_ON);
    rstTrackRate = false;
  } else {
    moreButton.draw(TRACK_R_Y+y_offset, "RstTrack", BUT_OFF);
  }   

  // Show current Tracking Rate
  // For ALT/AZ this always shows the default rate
  y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER+12;
 
  sprintF(reply, "%0.5f", siderealToHz(mount.trackingRate));
  canvMoreInsPrint.printRJ(TRACK_R_X-6, TRACK_R_Y+y_offset, 85, 15, reply, false);

  // ---- right hand column of buttons ----
  y_offset = 0;
  // Filter Selection Button - circular selection of 3 values
  if (filterBut) {
    moreButton.draw(FM_X, FM_Y + y_offset, FM_BOXSIZE_X, FM_BOXSIZE_Y, activeFilterStr[activeFilter], BUT_ON);
    if (activeFilter == FM_ALIGN_ALL_SKY && !objectSelected) {
      canvMoreInsPrint.printRJ(2, 472, 317, 15, " All Sky For STARS only", true);
    } else {
      canvMoreInsPrint.printRJ(2, 472, 317, 15, "", false);
    }
    filterBut = false;
  } else {
    moreButton.draw(FM_X, FM_Y + y_offset, FM_BOXSIZE_X, FM_BOXSIZE_Y, activeFilterStr[activeFilter], BUT_OFF);
  }

  // Clear Custom Catalog
  y_offset += FM_BOXSIZE_Y + FM_SPACER_Y;
  if (clrCustom) {
    if (!yesCancelActive) {
      yesCancelActive = true;
      moreButton.draw(MISC_X, MISC_Y + y_offset, MISC_BOXSIZE_X/2, MISC_BOXSIZE_Y, "Yes", BUT_ON);
      moreButton.draw(MISC_X+MISC_BOXSIZE_X/2, MISC_Y + y_offset, MISC_BOXSIZE_X/2, MISC_BOXSIZE_Y, "Cancel", BUT_ON);
      if (!objectSelected) canvMoreInsPrint.printRJ(2, 472, 317, 15, " Delete Custom Catalog?!", true);
    }
    if (yesBut) { // go ahead and clear
      moreButton.draw(MISC_X, MISC_Y + y_offset, MISC_BOXSIZE_X, MISC_BOXSIZE_Y, "Clearing", BUT_ON);
      
      File rmFile = SD.open("/custom.csv");
        if (rmFile) {
          SD.remove("/custom.csv");
        }
      rmFile.close(); 

      yesBut = false;
      clrCustom = false;
      yesCancelActive = false;
    }
    if (cancelBut) {
      cancelBut = false;
      clrCustom = false;
      yesCancelActive = false;
      moreButton.draw(MISC_X, MISC_Y + y_offset, MISC_BOXSIZE_X, MISC_BOXSIZE_Y, "Clr Custom", BUT_OFF);
      canvMoreInsPrint.printRJ(2, 472, 317, 15, "", false);
    }
  } else {
    moreButton.draw(MISC_X, MISC_Y + y_offset, MISC_BOXSIZE_X, MISC_BOXSIZE_Y, "Clr Custom", BUT_OFF);
  }

  // Buzzer Enable Button
  y_offset += MISC_BOXSIZE_Y + MISC_SPACER_Y;
  if (soundEnabled) {
    moreButton.draw(MISC_X, MISC_Y + y_offset, MISC_BOXSIZE_X, MISC_BOXSIZE_Y, "Buzzer On", BUT_ON);
  } else { //off
    moreButton.draw(MISC_X, MISC_Y + y_offset, MISC_BOXSIZE_X, MISC_BOXSIZE_Y, "Buzzer Off", BUT_OFF);
  }

  // Larger Button Text for GoTo and Abort
  //tft.setFont(&FreeSansBold9pt7b); 
  tft.setFont(&UbuntuMono_Bold11pt7b); 
  // Go To Coordinates Button
  if (goToButton) {
    moreLgButton.draw(GOTO_BUT_X, GOTO_BUT_Y,  GOTO_M_BOXSIZE_X, GOTO_M_BOXSIZE_Y, "Slewing", BUT_ON);
    goToButton = false;
  } else {
    if (!mount.isSlewing()) { 
      moreLgButton.draw(GOTO_BUT_X, GOTO_BUT_Y,  GOTO_M_BOXSIZE_X, GOTO_M_BOXSIZE_Y, "Go To", BUT_OFF);
    }
  }
  
  // Abort GoTo Button
  if (abortPgBut) {
    moreLgButton.draw(ABORT_M_BUT_X, ABORT_M_BUT_Y, GOTO_M_BOXSIZE_X, GOTO_M_BOXSIZE_Y, "Aborting", BUT_ON);
    abortPgBut = false;
  } else {
    moreLgButton.draw(ABORT_M_BUT_X, ABORT_M_BUT_Y, GOTO_M_BOXSIZE_X, GOTO_M_BOXSIZE_Y, "Abort", BUT_OFF);
  }

  tft.setFont(&Inconsolata_Bold8pt7b); 

  // Draw the Catalog Buttons
  char title[16]="";
  y_offset = 0;
  for (uint16_t i=1; i<=cat_mgr.numCatalogs(); i++) {
    cat_mgr.select(i-1);
    strcpy(title,cat_mgr.catalogTitle());
    moreButton.draw(CAT_SEL_X, CAT_SEL_Y+y_offset, CAT_SEL_BOXSIZE_X, CAT_SEL_BOXSIZE_Y, title, BUT_OFF);
    y_offset += CAT_SEL_SPACER;
  }

  // Planet Catalog Button
  moreButton.draw(CAT_SEL_X, CAT_SEL_Y+y_offset, CAT_SEL_BOXSIZE_X, CAT_SEL_BOXSIZE_Y, "Planets", BUT_OFF);

  y_offset += CAT_SEL_SPACER;
  // Treasure Catalog
  moreButton.draw(CAT_SEL_X, CAT_SEL_Y+y_offset, CAT_SEL_BOXSIZE_X, CAT_SEL_BOXSIZE_Y, "Treasure", BUT_OFF);

  y_offset += CAT_SEL_SPACER;
  // Custom User Catalog Button
  moreButton.draw(CAT_SEL_X, CAT_SEL_Y+y_offset, CAT_SEL_BOXSIZE_X, CAT_SEL_BOXSIZE_Y, "Custom Cat", BUT_OFF);
}

//==============================================
// ===== TouchScreen Poll "MORE" page ==========
//==============================================
bool MoreScreen::touchPoll(uint16_t px, uint16_t py) {

  // Home Page ICON Button
  if (px > 10 && px < 50 && py > 5 && py < 37) {
    BEEP;
    setCurrentScreen(HOME_SCREEN); 
    homeScreen.draw();
    return false; // don't update this screen (MORE)
  }

  // Select Tracking Rates
  int x_offset = 0;
  int y_offset = 0;

  // Sidereal Rate 
  if (py > TRACK_R_Y+y_offset && py < (TRACK_R_Y+y_offset + TRACK_R_BOXSIZE_Y) && px > TRACK_R_X && px < (TRACK_R_X + TRACK_R_BOXSIZE_X)) {
    BEEP;
      setLocalCmd(":TQ#");
      sidereal = true;
      lunarRate = false;
      kingRate = false;
      return true;
  }
  // Lunar Rate
  y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER;
  if (py > TRACK_R_Y+y_offset && py < (TRACK_R_Y+y_offset + TRACK_R_BOXSIZE_Y) && px > TRACK_R_X && px < (TRACK_R_X + TRACK_R_BOXSIZE_X)) {
    BEEP;
      setLocalCmd(":TL#");
      sidereal = false;
      lunarRate = true;
      kingRate = false;
      return true;
  }
  // King Rate
  y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER;
  if (py > TRACK_R_Y+y_offset && py < (TRACK_R_Y+y_offset + TRACK_R_BOXSIZE_Y) && px > TRACK_R_X && px < (TRACK_R_X + TRACK_R_BOXSIZE_X)) {
    BEEP;
      setLocalCmd(":TK#");
      sidereal = false;
      lunarRate = false;
      kingRate = true;
      return true;
  }
  // Increment tracking rate
  y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_GROUP_SPACER ;
  if (py > TRACK_R_Y+y_offset && py < (TRACK_R_Y+y_offset + TRACK_R_BOXSIZE_Y) && px > TRACK_R_X && px < (TRACK_R_X + TRACK_R_BOXSIZE_X)) {
    BEEP;
      setLocalCmd(":T+#");
      incTrackRate = true;
      decTrackRate = false;
      return true;
  }
  // Decrement tracking rate
  y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_SPACER;
  if (py > TRACK_R_Y+y_offset && py < (TRACK_R_Y+y_offset + TRACK_R_BOXSIZE_Y) && px > TRACK_R_X && px < (TRACK_R_X + TRACK_R_BOXSIZE_X)) {
    BEEP;
      setLocalCmd(":T-#");
      incTrackRate = false;
      decTrackRate = true;
      return true;
  }
  // Reset tracking rate
  y_offset += TRACK_R_BOXSIZE_Y+TRACK_R_GROUP_SPACER ;
  if (py > TRACK_R_Y+y_offset && py < (TRACK_R_Y+y_offset + TRACK_R_BOXSIZE_Y) && px > TRACK_R_X && px < (TRACK_R_X + TRACK_R_BOXSIZE_X)) {
    BEEP;
      setLocalCmd(":TR#");
      rstTrackRate = true;
      return true;
  }

  y_offset = 0;
  // Filter Select Button
  if (px > FM_X + x_offset && px < FM_X + x_offset + FM_BOXSIZE_X && py > FM_Y + y_offset && py <  FM_Y + y_offset + FM_BOXSIZE_Y) {
    BEEP;
    filterBut = true; 
    // circular selection
    if (activeFilter == FM_NONE) {
      activeFilter = FM_ABOVE_HORIZON; // filter disallows alt < 10 deg
      cat_mgr.filterAdd(activeFilter); 
      return true;
    }

    if (activeFilter == FM_ABOVE_HORIZON) {
      activeFilter = FM_ALIGN_ALL_SKY; // Used for stars only here: filter only allows Mag>=3; Alt>=10; Dec<=80
      cat_mgr.filterAdd(activeFilter);
      return true; 
    }

    if (activeFilter == FM_ALIGN_ALL_SKY) {
      activeFilter = FM_NONE;  // no filter
      cat_mgr.filtersClear();
      return true;
    }
  }

  y_offset = 0;
  // Clear Custom Button
  y_offset += FM_BOXSIZE_Y + FM_SPACER_Y;
  if (!yesCancelActive && px > MISC_X + x_offset && px < MISC_X + x_offset + MISC_BOXSIZE_X && py > MISC_Y + y_offset && py <  MISC_Y + y_offset + MISC_BOXSIZE_Y) {
    BEEP;
    clrCustom = true;
    return true;
  }
  // Clearing Custom Catalog "Yes"
  if (px > MISC_X && px < MISC_X + 30 && py > MISC_Y + y_offset && py <  MISC_Y + y_offset + MISC_BOXSIZE_Y) {
    BEEP;
    yesBut = true;
    clrCustom = true;
    return true;
  }
  // Clearing Custom catalog "Cancel"
  if (px > MISC_X + 40 && px < MISC_X + 120 && py > MISC_Y + y_offset && py <  MISC_Y + y_offset + MISC_BOXSIZE_Y) {
    BEEP;
    cancelBut = true;
    clrCustom = true;
    return true;
  }

  // Buzzer Button
  y_offset += MISC_BOXSIZE_Y + MISC_SPACER_Y;
  if (px > MISC_X + x_offset && px < MISC_X + x_offset + MISC_BOXSIZE_X && py > MISC_Y + y_offset && py <  MISC_Y + y_offset + MISC_BOXSIZE_Y) {
    BEEP;
    if (!soundEnabled) {
      soundEnabled = true; // turn on
      setLocalCmd(":SX97,1#");
    } else {
      soundEnabled = false; // toggle off
      setLocalCmd(":SX97,0#");
    }
    return true;
  }
  
  // **** Go To Target Coordinates ****
  if (py > GOTO_BUT_Y && py < (GOTO_BUT_Y + GOTO_M_BOXSIZE_Y) && px > GOTO_BUT_X && px < (GOTO_BUT_X + GOTO_M_BOXSIZE_X)) {
    BEEP;
    goToButton = true;
    setLocalCmd(":MS#");
  return true;
  }

  // **** ABORT GOTO ****
  if (py > ABORT_M_BUT_Y && py < (ABORT_M_BUT_Y + GOTO_M_BOXSIZE_Y) && px > ABORT_M_BUT_X && px < (ABORT_M_BUT_X + GOTO_M_BOXSIZE_X)) {
    BEEP;
    soundFreq(1500, 200);
    abortPgBut = true;
    setLocalCmd(":Q#"); // stops move
    motor1.enable(false); // do this for safety reasons...mount may be colliding with something
    axis1.enable(false);
    motor2.enable(false);
    axis2.enable(false);
    return true;
  }

  // SHC Catalog Selection Buttons 
  y_offset = 0;
  for (uint16_t i=1; i<=cat_mgr.numCatalogs(); i++) {
    if (px > CAT_SEL_X && px < CAT_SEL_X + CAT_SEL_BOXSIZE_X && py > CAT_SEL_Y+y_offset  && py < CAT_SEL_Y+y_offset + CAT_SEL_BOXSIZE_Y) {
      BEEP;
      // disable ALL_SKY filter if any DSO catalog...it's for STARS only
      if (i != 1 && activeFilter == FM_ALIGN_ALL_SKY) { // 1 is STARS
        cat_mgr.filtersClear();
        activeFilter = FM_NONE;
      } 
      shcCatScreen.init(i-1); // draws the selected SHC Catalog page
      catalogsActive = true;
      return false; // shut off flag that draws More Page buttons

    }
    y_offset += CAT_SEL_SPACER;
  }

// Planet Catalog Select Button
  if (px > CAT_SEL_X && px < CAT_SEL_X + CAT_SEL_BOXSIZE_X && py > CAT_SEL_Y+y_offset  && py < CAT_SEL_Y+y_offset + CAT_SEL_BOXSIZE_Y) {
    BEEP;
    planetsScreen.draw(); // draws the Planets Catalog page
    catalogsActive = true;
    return false; // shut off flag that draws More Page buttons
  }

  // Treasure catalog select Button
  y_offset += CAT_SEL_SPACER;
  if (px > CAT_SEL_X && px < CAT_SEL_X + CAT_SEL_BOXSIZE_X && py > CAT_SEL_Y+y_offset  && py < CAT_SEL_Y+y_offset + CAT_SEL_BOXSIZE_Y) {
    BEEP;
    treasureCatScreen.init(); // draws the Treasure catalog page
    catalogsActive = true;
    return false; // shut off flag that draws More Page buttons
  }

  // User custom catalog select Button
  y_offset += CAT_SEL_SPACER;
  if (px > CAT_SEL_X && px < CAT_SEL_X + CAT_SEL_BOXSIZE_X && py > CAT_SEL_Y+y_offset  && py < CAT_SEL_Y+y_offset + CAT_SEL_BOXSIZE_Y) {
    BEEP;
    customCatScreen.init(); // draws the Custom Catalog page
    catalogsActive = true;
    return false; // shut off flag that draws More Page buttons
  }
  return false; // screen touched but not on a button
}

MoreScreen moreScreen;
