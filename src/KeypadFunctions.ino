/*F**************************************************************************
* NAME: readKey
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Read Keypad to configure Ventilator
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
#include "Definitions.h"



void readKey()
{
  String stringKey;
  char customKey = 0;//'\0';
  
  // check if data has been sent from the computer:
  if (Serial.available()) {
    // read the most recent byte (which will be from 0 to 255):
    customKey = Serial.read();
  }
  else{
    customKey = customKeypad.getKey();
  }

  if (customKey)
  {
    stringKey = "customKey:";
    stringKey = stringKey + String(customKey);
    DEBUG(stringKey);
    switch(customKey)
    {
      case '*':
        stateKey = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Input Your Type:Value");
        lcd.setCursor(0,1);
        lcd.print("1.MODE 2.RPM 3.IE");
        lcd.setCursor(0,2);
        lcd.print("4.PIP 5.PEEP 6.PO");
        lcd.setCursor(0,3);
        lcd.print("7.PR 8.VOL");
        stringKey = "Input Your Type:Value";
        DEBUG(stringKey);
      break;
      case '#':
        if(stateKey == 1){
          updateDisplay();
        }
        else{
          inputCode[pos] = '\0';
          if(pos <= 3){
            int valData = atoi(inputCode);
            stringKey = "value valData:";
            stringKey = stringKey + String(valData);
            DEBUG(stringKey);
            bool check = checkVal(valType, valData);
          }
          else
          {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("  Input Error!  ");
            lcd.setCursor(0, 1);
            lcd.print("  Please Again  ");
            delay(2000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("    Welcome!    ");
          }
        }
        stateKey = 0;
      break;
      default:
        if(stateKey == 1){
          valType = customKey; 
          lcd.clear();
          String stringTwo = checkType(customKey);
          stringKey = "valType:";
          stringKey = stringKey + stringTwo;
          DEBUG(stringKey);
          lcd.setCursor(0,0);
          lcd.print(stringTwo);
          pos = 0;
          stateKey = 2;
        }
        else if(stateKey == 2){
          inputCode[pos] = customKey;
          lcd.setCursor(pos+6,0);
          lcd.print(inputCode[pos]);
          pos ++;
          if(pos > 3){
            stateKey = 0;
            lcd.setCursor(0,1);
            lcd.print("Error Value");
          }
        }
    }
  }
}

bool checkVal(byte valType, int valValue){

  bool checkVal= false;

  lcd.setCursor(0,1);
  
  switch(valType){
      case MDTYPE:  // Ventilator Mode
        tempMode = (VentMode)valValue;
        if((tempMode >= VentMode::CV) && (tempMode <= VentMode::CPA)){
          currentVentMode = tempMode;
          lcd.print("MODE:");
          lcd.print(currentVentMode);
          checkVal = true;
        }
        else{
          lcd.print("ERROR MODE");
          checkVal = false;
        }
      break;
      case RPMTYPE:  // RPM
        if((valValue >= RPMMIN) && (valValue <= RPMMAX)){
          RPMVal = valValue;
          lcd.print("RPM:");
          lcd.print(RPMVal);
          checkVal = true;
        }
        else{
          lcd.print("ERROR RPM");
          checkVal = false;
        }
      break;
      case IETYPE:  // IE
        if((valValue >= IEMIN) && (valValue <= IEMAX)){
          IEVal = valValue;
          lcd.print("IE:");
          lcd.print(IEVal);
          checkVal = true;
        }
        else{
          lcd.print("ERROR IE");
          checkVal = false;
        }
      break;
      
      case PIPTYPE: // PIP
        if((valValue >= PIPMIN) && (valValue <= PIPMAX)){
          PIPVal = valValue;
          lcd.print("PIP:");
          lcd.print(PIPVal);
          checkVal = true;
        }
        else{
          lcd.print("ERROR PIP");
          checkVal = false;
        }
      break;
      case PEEPTYPE: // PEEP
        if((valValue >= PEEPMIN) && (valValue <= PEEPMAX)){
          if(PEEPVal < PIPVal){
            PEEPVal = valValue;
            lcd.print("PEEP:");
            lcd.print(PEEPVal);
            checkVal = true;
          }
          else{
            lcd.print("PEEP >= PEEP");
            lcd.print(valValue);
            checkVal = false;
          }
        }
        else{
          lcd.print("ERROR PEEP");
          checkVal = false;
        }
      break;
      case POTYPE:  // PO
        if((valValue >= POMIN) && (valValue <= POMAX)){
          POVal = valValue;
          lcd.print("PO:");
          lcd.print(POVal);
          checkVal = true;
        }
        else{
          lcd.print("ERROR PO");
          checkVal = false;
        }
      break;
      case PRTYPE:  // PR
        if((valValue >= PRMIN) && (valValue <= PRMAX)){
          PRVal = valValue;
          lcd.print("PR:");
          lcd.print(PRVal);
          checkVal = true;
        }
        else{
          lcd.print("ERROR PR");
          checkVal = false;
        }
      break;
      case VOLTYPE:  // VOL
        if((valValue >= VOLMIN) && (valValue <= VOLMAX)){
          VOLVal = valValue;
          lcd.print("VOL:");
          lcd.print(VOLVal);
          checkVal = true;
        }
        else{
          lcd.print("ERROR VOL");
          checkVal = false;
        }
      break;
      default:
         lcd.print("ERROR TYPE");
         checkVal = false;
      break;
    }
  return checkVal;
}

String checkType(byte valType){

  String stringOne = "Hello";

  switch(valType){
      case MDTYPE:  // MODE
          stringOne = String("MODE:");
      break;
      case RPMTYPE:  // RPM
          stringOne = String("RPM:");
      break;
      case IETYPE:  // IE
          stringOne = String("IE:");
      break;
      case PIPTYPE: // PIP
          stringOne = String("PIP:");
      break;
      case PEEPTYPE: // PEEP
           stringOne = String("PEEP:");
      break;
      case POTYPE:  // PO
          stringOne = String("PO:");
      break;
      case PRTYPE:  // PR
          stringOne = String("PR:");
      break;
      case VOLTYPE:  // VOL
          stringOne = String("VOL:");
      break;
      default:
         stringOne = String("ERROR TYPE");
    }
  return stringOne;
}
