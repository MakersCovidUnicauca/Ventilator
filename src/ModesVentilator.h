#ifndef VENTILATOR_MODES
#define VENTILATOR_MODES

enum VentMode
{
  CV,  //CONTROL POR VOLUMEN
  CVA, //CONTROL FOR VOLUMEN ASISTIDO
  CP,  //CONTROL POR PRESION
  CPA  //CONTROL POR PRESION ASISTIDA
};

enum AlarmType
{
  PWR, //CONTROL POR VOLUMEN
  OXY, //CONTROL FOR VOLUMEN ASISTIDO
  ALM, //CONTROL POR PRESION
  TEST //CONTROL POR PRESION ASISTIDA
};

enum SMState
{
  INIT,
  CONFIG,
  INHALE,
  PAUSE,
  EXHALE,
  LISTEN,
  EXLPAS
};

enum SMInput
{
  Unknown,
  BtnConfig,
  TIEnd,
  THEnd,
  TEEnd,
  TLEnd,
  TPEnd,
  BtnReset
};

#endif