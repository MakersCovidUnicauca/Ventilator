#ifndef Menu_h
#define Menu_h

#include "Definitions.h"

#include <RotaryEncoder.h>

// Setup a RoraryEncoder for pins 12 and 11:
RotaryEncoder encoder(ENCODER_DT_PIN, ENCODER_CLK_PIN);
int RotPosition = 20; 

/////////////////////////////////// Button
//////////////////////////////////////////////////
#include <ButtonDebounce.h>
ButtonDebounce buttEnc(ENCODER_SW_PIN, 150); //HIGH = config LOW = Normal

int menuitem = 1;
int frame = 1;
int page = 1;
int lastMenuItem = 1;
int posCursor = 1;
bool change = true;

String menuItem1 = "MODE";
String menuItem2 = "RPM";
String menuItem3 = "IE";
String menuItem4 = "PIP";
String menuItem5 = "PEEP";
String menuItem6 = "PO";
String menuItem7 = "PR";
String menuItem8 = "VOLUMEN";

String ventMode[4] = { "CV", "CVA", "CP", "CPA" };
//int currentVentMode  = 0;

boolean up = false;
boolean down = false;
boolean middle = false;

void readEncoderUpdate();
void buttEncChanged(int state);
void updateMenu();
void drawMenu();
void setBlinkCursor(int pos);
void resetDefaults();
void displayIntMenuPage(String menuItem, int value);
void displayStringMenuPage(String menuItem, String value);
void displayMenuItem(String item, int position, boolean selected);

#endif