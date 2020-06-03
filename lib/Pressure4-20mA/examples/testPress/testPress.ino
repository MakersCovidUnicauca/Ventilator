#include <Pressure.h>

Pressure pInhale(A0);
Pressure pExhale(A1);

void setup()
{
}

void loop()
{
  pInhale.readRaw();
  pInhale.readCmH2O();
  pExhale.readRaw();
  pExhale.readCmH2O();
  delay(3000);
}
