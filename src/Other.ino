
#include "Definitions.h"



void buttonChanged(int state){
  String stringBtn;
  if((state == HIGH) && (currentState == INIT) ){
    currentInput = SMInput::BtnReset;
    stringBtn = "input_reset";
  }
  else if((state == LOW) && (currentState == CONFIG) ){
    currentInput = SMInput::BtnConfig;
    stringBtn = "BtnConfig";
  }
  String stringone = "Changed:";
  stringone = stringone + String(state) + stringBtn;
  DEBUG(stringone);
}

//Write the value to the appropriate byte of the EEPROM.
void storeVarVent(){

  EEPROM.write(addrVarVent, MRK_STR_VAR);
  EEPROM.write(MDTYPE, currentVentMode);
  EEPROM.write(RPMTYPE, RPMVal);
  EEPROM.write(IETYPE, IEVal);

  EEPROM.write(PIPTYPE, PIPVal);
  EEPROM.write(PEEPTYPE, PEEPVal);
  EEPROM.write(POTYPE, POVal);
  EEPROM.write(PRTYPE, PRVal);
  EEPROM.put(VOLTYPE, VOLVal);
}

// read a byte from the current address of the EEPROM
void readVarVent(){

  byte value = EEPROM.read(addrVarVent);
  if(value == MRK_STR_VAR){
    currentVentMode = (VentMode)EEPROM.read(MDTYPE);
    RPMVal = EEPROM.read(RPMTYPE);
    IEVal = EEPROM.read(IETYPE);
    
    PIPVal = EEPROM.read(PIPTYPE);
    PEEPVal = EEPROM.read(PEEPTYPE);
    POVal = EEPROM.read(POTYPE);
    PRVal = EEPROM.read(PRTYPE);
    EEPROM.get(VOLTYPE, VOLVal);
  }
}
