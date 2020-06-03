/*
  Pressure.cpp - Library for pressure sensor analog.
  Created by Fulvio Y. Vivas, April 25, 2020.
  Released into the public domain.
*/

#include "Pressure.h"

Pressure::Pressure(int pin)
{
  _pin = pin;
}

int Pressure::readRaw()
{
  _sensorValue = analogRead(_pin);
  return _sensorValue;
}

float Pressure::readCmH2O()
{
  _sensorValue = analogRead(_pin);
#ifdef ESP32_PCB
  _pressVal = (_sensorValue *0.03540)- 17.59445 ;
#else
    _pressVal = (_sensorValue *0.10733)- 17.60358 ;
#endif
  return _pressVal;
}

