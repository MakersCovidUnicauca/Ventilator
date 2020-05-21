#ifndef LIBRARIESDEFS
#define LIBRARIESDEFS

#include "PinOutVent.h"

///// Keypad
//////////////////////////////////////////////////
#include "Keypad.h"
const byte ROWS = 4;
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte rowPins[ROWS] = {KEY_R1_PIN, KEY_R2_PIN, KEY_R3_PIN, KEY_R4_PIN};
byte colPins[COLS] = {KEY_C1_PIN, KEY_C2_PIN, KEY_C3_PIN};
int stateKey = 0;
int pos = 0;
char inputCode[4] = {'1', '0', '3', '0'};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
/////////////////////////////////// Button
//////////////////////////////////////////////////
#include <ButtonDebounce.h>
ButtonDebounce button(BTN_CFG_PIN, 250); //HIGH = config LOW = Normal
void buttonChanged(int state);

/////////////////////////////////// LiquidCrystal
//////////////////////////////////////////////////

#include <LiquidCrystal_PCF8574.h>
LiquidCrystal_PCF8574 lcd(0x27); //Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
/////////////////////////////////// State machine
//////////////////////////////////////////////////

/////////////////// EEPROM
#include <EEPROM.h>
/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int addrVarVent = 0x30;

#define MRK_STR_VAR 0xAA // Mark Store EEprom variable ventilator

#endif