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

  char float_str0[4];
  char float_str1[4];
  char float_str2[4];
  char line3[21];
  dtostrf(pressInhale, 2, 0, float_str0);
  dtostrf(pressExhale, 2, 0, float_str1);
  dtostrf(pressPlateau, 2, 0, float_str2);

  // Now you can sprintf
  sprintf(line3, "PI:%-3sPP:%-3sPE:%-4s", float_str0, float_str1, float_str2); // %3s right pads the string
  //Serial.println (line3);
  lcd.setCursor(0, 3);
  lcd.print(line3);
}
