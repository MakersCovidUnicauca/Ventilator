/*
  Pressure.h - Library for pressure sensor analog.
  Created by Fulvio Y. Vivas, April 25, 2020.
  Released into the public domain.
*/

#ifndef Pressure_h
#define Pressure_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class Pressure
{
  public:
    Pressure(int pin);
    int readRaw();
    float readCmH2O();
  private:
    int _pin;
	int _sensorValue;
	float _pressVal;
};

#endif

