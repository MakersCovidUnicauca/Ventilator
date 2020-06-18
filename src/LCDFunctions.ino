/*!
\file   LCDFunctions.ino
\date   2020-05-05
\author Fulvio Vivas <fulvio.vivas@gmail.com>
\brief  LCD.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of XXXXXX, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) XXXXX 2020. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/

/*F**************************************************************************
* NAME: updateDisplay
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Display Ventilator Parameter on LCD
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
#include "Definitions.h"

void InitLCDVent(){
  lcd.begin(20, 4);
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("Ventilador Pube ");
  lcd.setCursor(0, 1);
  lcd.print("Init Wifi");
}


void refMotorLCD(){
  // set up the LCD's number of columns and rows:
  lcd.home();
  lcd.clear();
  lcd.print("Ventilador Pube ");
  lcd.setCursor(0, 1);
  lcd.print("ref Motor");
  
}

void mainMsg(){
  lcd.home();
    lcd.clear();
    lcd.print("Ventilador ");
    lcd.setCursor(0, 1);
    lcd.print("Init ");
    lcd.setCursor(0, 2);
    lcd.print(WiFi.localIP());
}

void updateDisplay(void)
{
  lcd.clear();
  // Cursor en la primera posición de la 1° fila
  lcd.setCursor(0, 0);
  lcd.print("MODE:");
  lcd.print(currentVentMode);
  // Cursor en la 11° posición de la 1° fila
  lcd.setCursor(11, 0);
  lcd.print("VOL:");
  lcd.print(VOLVal);
  // Cursor en la primera posición de la 2° fila
  lcd.setCursor(0, 1);
  lcd.print("RPM:");
  lcd.print(RPMVal); //1 decimal
  // Cursor en la 7° posición de la 2° fila
  lcd.setCursor(7, 1);
  lcd.print("IE:");
  lcd.print(IEVal);
  lcd.setCursor(12, 1);
  lcd.print("PEEP:");
  lcd.print(PEEPVal);
  // Cursor en la primera posición de la 3° fila
  lcd.setCursor(0, 2);
  lcd.print("PIP:");
  lcd.print(PIPVal);
  // Cursor en la 7° posición de la 3° fila
  lcd.setCursor(7, 2);
  lcd.print("PO:");
  lcd.print(POVal);
  // Cursor en la 7° posición de la 3° fila
  lcd.setCursor(13, 2);
  lcd.print("PR:");
  lcd.print(PRVal);
  // Cursor en la primera posición de la 4° fila
  lcd.setCursor(0, 3);
  lcd.print("PI:");
  lcd.print(pressInhale);
  // Cursor en la 6° posición de la 4° fila
  lcd.setCursor(6, 3);
  lcd.print("PP:");
  lcd.print(pressPlateau);
  // Cursor en la 7° posición de la 4° fila
  lcd.setCursor(12, 3);
  lcd.print("PE:");
  lcd.print(pressExhale);

#ifdef __DEBG__
  Serial.print("PIP:");
  Serial.print(PIPVal);
  Serial.print(", PEEP:");
  Serial.print(PEEPVal);
  Serial.print(", RPM:");
  Serial.print(RPMVal);
  Serial.print(", VOL:");
  Serial.print(VOLVal);
  Serial.print(", IE:");
  Serial.print(IEVal);
  Serial.print(", PO:");
  Serial.print(POVal);
  Serial.print(", PR:");
  Serial.println(PRVal);
#endif
}

void updateDisplayPressure()
{

  char float_str0[6];
  char line0[21];

  lcd.home();
  lcd.clear();
  lcd.print("Operando");
  lcd.setCursor(0, 1);
  dtostrf(pressInhale, 2, 2, float_str0);
  sprintf(line0, "PInhale: %-5s", float_str0); // %3s right pads the string
  lcd.print(line0);

  lcd.setCursor(0, 2);
  dtostrf(pressPlateau, 2, 2, float_str0);
  sprintf(line0, "PPlateau: %-5s", float_str0); // %3s right pads the string
  lcd.print(line0);

  lcd.setCursor(0, 3);
  dtostrf(pressExhale, 2, 2, float_str0);
  sprintf(line0, "PExhale: %-5s", float_str0); // %3s right pads the string
  lcd.print(line0);

}
