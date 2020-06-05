////////////////maquinas de estado//////////////////////
//

#include "Definitions.h"

////Simulation sin btnConfig
void waitConfig()
{
  DEBUG("TI_CONFIG");
  currentInput = SMInput::BtnConfig;
}

void timeoutTI()
{
  DEBUG("TI_END");
  currentInput = SMInput::TIEnd;
}

void timeoutTH()
{
  DEBUG("TH_END");
  currentInput = SMInput::THEnd;
}

void timeoutTE()
{
  DEBUG("TE_END");
  currentInput = SMInput::TEEnd;
}

void measurePress()
{

  pAmbu = pressAmbu.readCmH2O() - offset;
  pressureUser = pressUser.readCmH2O() - offset1;
   
  if (FlagAire == true)
  {
    if(pAmbu == 0.0)
    {
      DEBUG("END OXY+AIR");
      //cerrar valvula de aire
      FlagAire = false;
      //digitalWrite(VALV_EXTR_PIN, LOW);     //cerrar valvula de oxigeno
    }
  }


  if (FlagOxig == true)
  {
    if(pAmbu == 0.0){
      calculeAir();
    }
    float pOxig = pressOxig.readCmH2O();
    if (pOxig < PRVal)
    {
      //alarma(AlarmType::OXY);
    }
  }

  FlagPressure = true;

#ifdef Graphic_Serial
    Serial.print(pInh, 3);
    Serial.print(",");
    Serial.print(GetPosition());
    Serial.println(" ");
#endif
  preUser[TimestoSend] = pAmbu;
  volUser[TimestoSend] = GetPosition();
  TimestoSend++; 
  if (TimestoSend >= TimeSendGraphic)
  {
    TimestoSend = 0;
#ifdef __DEBG__
    //DEBUG("PRESSURE");
    //Serial.print("PInh: ");
    //Serial.print(pInh, 3);
    //Serial.print(", PExh: ");
    //Serial.print(pressureTemp, 3);
    //Serial.print(", POxig: ");
    //Serial.println(pOxig, 3);
#endif
  sendGraphicFlag = 1;
  }
  
}

void CtrlPressure()
{
  if (currentVentMode == VentMode::CP)
  {
    if (pressureUser >= PIPVal)
    {
      DEBUG("PIP_DETEC");
      Motor.stop();
      asyncTask2.Stop();
      asyncTask3.Start();
      currentInput = SMInput::TIEnd;
    }
  }
}

void MngAssitExh()
{
  if (FlagPressure)
  {
    if (pressureUser < pressExhale)
    {
      pressExhale = pressureUser;
    }
    if ((currentVentMode == VentMode::CVA) || (currentVentMode == VentMode::CPA))
    {
      if (pressExhale >= PLI_FAB)
      {
        DEBUG("PLI_DETEC");
        asyncTask1.Stop();
        asyncTask2.Start();
        currentInput = SMInput::TEEnd;
      }
    }
    FlagPressure = false;
  }
}

void MngAssitInh()
{
  if (FlagPressure)
  {
    if (pressureUser > pressInhale)
    {
      pressInhale = pressureUser;
    }
    if ((currentVentMode == VentMode::CVA) || (currentVentMode == VentMode::CPA))
    {
      if (pressInhale >= PLI_FAB)
      {
        DEBUG("TI_END");
        asyncTask2.Stop();
        asyncTask3.Start();
        currentInput = SMInput::TIEnd;
      }
    }
    FlagPressure = false;
  }
}

void calculePlateau()
{
  if (FlagPressure)
  {
    //float pExh = pressExh.readCmH2O();
    if (pressureUser != pressPlateau)
    {
      pressPlateau = pressureUser;
    }
    else
    {
      idxPlateau++;
    }
    if (idxPlateau == 4)
    {
      String stringone = "Plateau:";
      stringone = stringone + String(pressPlateau);
      DEBUG(stringone);
    }
    FlagPressure = false;
  }
}

void calculeTime()
{
  DEBUG("CTIME");
  TVal = (TIMESEC / RPMVal);
  TIVal = TVal / (1 + IEVal);
  TEVal = TVal - TIVal;
  TI = TIVal - THVal;

  asyncTask1.SetIntervalMillis(TEVal);
  asyncTask2.SetIntervalMillis(TI);
  asyncTask3.SetIntervalMillis(THVal);

#ifdef __DEBG__
  Serial.print("T: ");
  Serial.print(TVal);
  Serial.print(" TI: ");
  Serial.print(TI);
  Serial.print(" TP: ");
  Serial.print(THVal);
  Serial.print(" TE: ");
  Serial.println(TEVal);
#endif
}

void alarma(byte alarm)
{
  String stringAlarm = "ALARMA:";
  String stringone;
  switch (alarm)
  {
  case PWR:
    stringone = "POWER";
    break;
  case OXY:
    stringone = "OXYGENO";
    break;
  case ALM:
    stringone = "ALARMA:";
    break;
  case TEST:
    stringone = "TEST:";
    break;
  }
  stringAlarm = stringAlarm + stringone;
  DEBUG(stringAlarm);
}

void calculeAir()
{
  if ((Motor.isRunning() == 0) && (FlagAire == false))
  {
    DEBUG("AIRE");
    float Vol;
    float Po = POVal * 0.01; //
    if ((currentVentMode == VentMode::CV) || (currentVentMode == VentMode::CVA))
    {
      Vol = VOLVal;
    }
    else if ((currentVentMode == VentMode::CP) || (currentVentMode == VentMode::CPA))
    {
      Vol = VOLMAX;
    }
    else{
      Vol = VOLVal;
    }

    mPosEnd = float(Vol / RELMMVOL);
    DistMotor = INITPOSITION - mPosEnd;
    VelMotor = float(mPosEnd / ((TIVal * (1 - Po)) / 1000.0));
    AcelMotor = VelMotor * 20;
    SetMotor(DistMotor, VelMotor, AcelMotor);

    //cerrar valvula de oxigeno
    digitalWrite(VALV_OXIG_PIN, LOW);
    FlagOxig = false;
    FlagAire = true;
  
#ifdef __DEBG__
    Serial.print(" DistMotor: ");
    Serial.print(DistMotor);
    Serial.print(" mPosEnd: ");
    Serial.print(mPosEnd);
    Serial.print(" Po: ");
    Serial.print(Po);
    Serial.print(" mPosOxi: ");
    Serial.print(mPosOxi);
    Serial.print(" DISTM: ");
    Serial.print(DistMotor);
    Serial.print(" VelM: ");
    Serial.print(VelMotor, 3);
    Serial.print(" AccelM: ");
    Serial.println(AcelMotor, 3);
#endif
  }
}

void calculeOxig()
{

  DEBUG("OXYGENO");
  float Vol;
  float Po = POVal * 0.01; //

  if ((currentVentMode == VentMode::CV) || (currentVentMode == VentMode::CVA))
  {
    VOLRes = (mPosCurrent - INITPOSITION) * RELMMVOL;
    VOLRes = 0;
    Vol = VOLVal - VOLRes;
  }
  else if ((currentVentMode == VentMode::CP) || (currentVentMode == VentMode::CPA))
  {
    Vol = VOLMAX;
  }
  else{
    Vol = VOLVal;
  }

  mPosOxi = Vol - float(((1 - Po) * Vol / 0.79));
  mPosOxi = (mPosOxi / RELMMVOL);
  DistMotor = INITPOSITION - mPosOxi;
  //VelMotor = mPosOxi / ((TIVal * Po) / 1000.0);
  VelMotor = mPosOxi / (TIVal / 1000.0);
  AcelMotor = VelMotor * 20;
  SetMotor(DistMotor, VelMotor, AcelMotor);

  //abrir valvula de oxigeno
  digitalWrite(VALV_OXIG_PIN, HIGH);
  FlagOxig = true;
 
  /////////////////test motor sin oxigeno/////////////////////////////
  /*
  mPosEnd = float(VOLVal / RELMMVOL);
  DistMotor = INITPOSITION - mPosEnd;
  Po = POVal * 0.01; //
  VelMotor = float(mPosEnd / ((TIVal * (1 - Po)) / 1000.0));
  AcelMotor = VelMotor * 20;
  SetMotor(DistMotor, VelMotor, AcelMotor);
  */

#ifdef __DEBG__
  Serial.print("VOLRes: ");
  Serial.print(VOLRes);
  Serial.print(" Vol: ");
  Serial.print(Vol);
  Serial.print(" Po: ");
  Serial.print(Po);
  Serial.print(" mPosOxi: ");
  Serial.print(mPosOxi);

  Serial.print(" DISTM: ");
  Serial.print(DistMotor);
  Serial.print(" VelM: ");
  Serial.print(VelMotor, 3);
  Serial.print(" AccelM: ");
  Serial.println(AcelMotor, 3);
#endif
}

void stateInit()
{
  if (currentInput == SMInput::BtnReset)
    changeState(SMState::CONFIG);
  if (currentInput == SMInput::BtnConfig) //Simulation sin btnConfig
    changeState(SMState::EXHALE);         ////Simulation sin btnConfig
}

// Acciones de los estados y condiciones de transiciones
void stateConfig()
{
  if (currentInput == SMInput::BtnConfig)
    changeState(SMState::EXHALE);
  
  #ifdef TEST_KEY
  readKey();
  #endif
  #ifdef TEST_ENC
  buttEnc.update();
  readEncoderUpdate();
  updateMenu();
  if (change == true){
    drawMenu();
    change = false;
  }
  #endif
}

void stateInhale()
{
  if (currentInput == SMInput::TIEnd)
    changeState(SMState::PAUSE);
  if (currentInput == SMInput::BtnReset)
    changeState(SMState::CONFIG);

#ifdef TEST_MODE
  CtrlPressure();
  MngAssitInh();
#endif
}

void statePause()
{
  if (currentInput == SMInput::THEnd)
    changeState(SMState::EXHALE);
  if (currentInput == SMInput::BtnReset)
    changeState(SMState::CONFIG);
#ifdef TEST_SENSOR
  calculePlateau();
#endif
}

void stateExhale()
{
  if (currentInput == SMInput::TEEnd)
    changeState(SMState::INHALE);
  if (currentInput == SMInput::BtnReset)
    changeState(SMState::CONFIG);
#ifdef TEST_MOTOR
  calculeAir();
#endif
#ifdef TEST_MODE
  MngAssitExh();
#endif
}

void stateListen()
{
}

void stateExlPas()
{
}

void functInit(void)
{
  DEBUG("INIT");
  mainMsg();
  button.setCallback(buttonChanged);
  //menu
  buttEnc.setCallback(buttEncChanged);
  ///
  asyncTask5.Start(); ////Simulation sin btnConfig
  //readVarVent();  ////Simulation sin btnConfig
}

void functConfig(void)
{
  DEBUG("CONFIG");
  readVarVent();
  drawMenu();
  change = false;
}

void functInhale(void)
{
  DEBUG("INHALE");
#ifdef TEST_LCD
  updateDisplayPressure();
#endif
#ifdef TEST_SENSOR
  pressInhale = pressureUser;
#endif
#ifdef TEST_MOTOR
  FlagAire = false;

  DistMotor = INITPOSITION;
  VelMotor = float(mPosEnd / ((TIVal / 1000.0)));
  AcelMotor = VelMotor * 20;
  SetMotor(DistMotor, VelMotor, AcelMotor);
#endif
}

void functPause(void)
{
  DEBUG("PAUSE");
#ifdef TEST_SENSOR
  pressPlateau = pressureUser;
  idxPlateau = 0;
#endif
}

void functExhale(void)
{
  DEBUG("EXHALE");
  if (currentInput == SMInput::BtnConfig)
  {
#ifdef TEST_MODE
    if ((PMVal == 0) && (currentVentMode >= VentMode::CP))
    {
      currentVentMode = VentMode::CV;
    }
#endif
    calculeTime();
#ifdef TEST_SENSOR
    asyncTask4.Start();
#endif
#ifdef TEST_LCD
    updateDisplay();
#endif
    storeVarVent();
  }
#ifdef TEST_SENSOR
  pressExhale = pressureUser;
#endif
#ifdef TEST_MOTOR
  calculeOxig();
#endif
  //DEBUG("EXHALE1");//30 MILLISECONDS
  asyncTask1.Start();
}

void functListen(void)
{
  DEBUG("LISTEN");
}

void functExhPas(void)
{
  DEBUG("EXHALE PAUSE");
}

void functDefault(void)
{
  DEBUG("ERROR STATE");
}

// Actualiza el estado de la maquina
void updateStateMachine()
{
  switch (currentState)
  {
  case INIT:
    stateInit();
    break;
  case CONFIG:
    stateConfig();
    break;
  case INHALE:
    stateInhale();
    break;
  case PAUSE:
    statePause();
    break;
  case EXHALE:
    stateExhale();
    break;
  case LISTEN:
    stateListen();
    break;
  case EXLPAS:
    stateExlPas();
    break;
  }
}

// Funcion que cambia el estado y dispara las transiciones
void changeState(int newState)
{
  currentState = (SMState)newState;

  switch (currentState)
  {
  case SMState::INIT: // state init
    functInit();
    break;
  case SMState::CONFIG: // state configuration
    functConfig();
    break;
  case SMState::INHALE: // state inhale
    functInhale();
    break;
  case SMState::PAUSE: // state exhale
    functPause();
    break;
  case SMState::EXHALE: // state pause
    functExhale();
    break;
  case SMState::LISTEN: // state listen
    functListen();
    break;
  case SMState::EXLPAS: // state exhale pause
    functExhPas();
    break;
  default:
    functDefault();
    break;
  }
}
