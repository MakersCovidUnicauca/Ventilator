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
   
  if (FlagOxig == true)
  {
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
  preUser[TimestoSend] = pressureUser;
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

void calculePlateauTest()
{
  if (FlagPressure)
  {
    pressureUser = pressUser.readCmH2O() - offset1;
    if (pressureUser <= (pressInhale/2))
    {
      idxPlateau++;
      if ((idxPlateau >= 4) && (flagPlateau == false))
      {
        pressPlateau = pressureUser;
        String stringone = "Plateau:";
        stringone = stringone + String(pressPlateau);
        DEBUG(stringone);
        flagPlateau = true;
      }
    }
    FlagPressure = false;
  }
}


void calculePlateau()
{
  if (FlagPressure)
  {
    pressureUser = pressUser.readCmH2O() - offset1;
    if (pressureUser != pressPlateau)
    {
      pressPlateau = pressureUser;
    }
    else
    {
      idxPlateau++;
    }
    if ((idxPlateau >= 4) && (flagPlateau == false))
    {
      String stringone = "Plateau:";
      stringone = stringone + String(pressPlateau);
      DEBUG(stringone);
      flagPlateau = true;
    }
    FlagPressure = false;
  }
}

void calculeTime()
{
  if ((currentVentMode == VentMode::CV) || (currentVentMode == VentMode::CP))
  {
    DEBUG("CTIME");
    TVal = (TIMESEC / RPMVal);
    TIVal = TVal / (1 + IEVal);
    TEVal = TVal - TIVal;
    TI = TIVal - THVal;

    asyncTaskTE.SetIntervalMillis(TEVal);
    asyncTaskTI.SetIntervalMillis(TI);
    asyncTaskTH.SetIntervalMillis(THVal);

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
  if ((currentVentMode == VentMode::CVS) || (currentVentMode == VentMode::CPS))
  {
    DEBUG("CTIME_ASSIT");
    TValAsst = (TIMESEC / RPMValAsst);  //10000
    TIValAsst = TValAsst / (1 + IEVal); //3333
    TEValAsst = TIValAsst * 6;
    THValAsst = TIValAsst * 6;
    TIAsst = TIValAsst * 6;

    asyncTaskTE.SetIntervalMillis(TEValAsst);
    asyncTaskTI.SetIntervalMillis(TIAsst);
    asyncTaskTH.SetIntervalMillis(THValAsst);
  }
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

void ctrlValvul(){
  if(mPosOxi == mPosCurrent){
    if (FlagOxig == false){
      //abrir valvula de oxigeno
      digitalWrite(VALV_OXIG_PIN, HIGH);
      FlagOxig = true;
      FlagAire = false;
    }
  }
  else if (mPosCurrent == mPosInit){
    if (FlagOxig == true){
      //cerrar valvula de oxigeno
      digitalWrite(VALV_OXIG_PIN, LOW);
      FlagOxig = false;
      FlagAire = false;
    }
  }
}

void calculePositions()
{
    DEBUG("AIRE");
    float Vol;
    float Po = POVal * 0.01; //
    
    VOLRes = (INITPOSITION - mPosCurrent) * RELMMVOL;
    Vol = VOLMAX - VOLRes;
    mPosVol = float(VOLVal / RELMMVOL);

    /*
    if ((currentVentMode == VentMode::CV) || (currentVentMode == VentMode::CVA))
    {
      mPosVol = float(VOLVal / RELMMVOL);
    }
    else if ((currentVentMode == VentMode::CP) || (currentVentMode == VentMode::CPA))
    {
    }
    else{
      Vol = VOLVal;
    }
    */

    mPosOxi = float(((1 - Po) * Vol / 0.79));
    mPosOxi = mPosCurrent - (mPosOxi / RELMMVOL);
 
    mPosEnd = float(Vol / RELMMVOL);
    DistMotor = mPosCurrent - mPosEnd;
    VelMotor = float(mPosEnd / (TIME_OXY_AIR / 1000.0));
    AcelMotor = VelMotor * 20;
    SetMotor(DistMotor, VelMotor, AcelMotor);

    //cerrar valvula de oxigeno
    digitalWrite(VALV_OXIG_PIN, LOW);
    FlagOxig = false;
    FlagAire = true;
  
#ifdef __DEBG__
    Serial.print(" mPosCurrent: ");
    Serial.print(mPosCurrent);
    Serial.print(" mPosEnd: ");
    Serial.print(mPosEnd);
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
  calculePressIns();
  CtrlVol();
  CtrlPIP();
//#ifdef TEST_MODE
  MngAssitInh();
  MngEspInh();
//#endif
}

void statePause()
{
  if (currentInput == SMInput::THEnd)
    changeState(SMState::EXHALE);
  if (currentInput == SMInput::BtnReset)
    changeState(SMState::CONFIG);
#ifdef TEST_SENSOR
  //calculePlateau();
  calculePlateauTest();
  CtrlPP();
#endif
}

void stateExhale()
{
  if (currentInput == SMInput::TEEnd)
    changeState(SMState::INHALE);
  if (currentInput == SMInput::BtnReset)
    changeState(SMState::CONFIG);
#ifdef TEST_MOTOR
  ctrlValvul();
#endif
//#ifdef TEST_MODE
  MngAssitExh();
  MngEspExh();
//#endif
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
  pressureUser = pressUser.readCmH2O() - offset1;
  pressInhale = pressureUser;
#endif
#ifdef TEST_MOTOR
  if ((currentVentMode == VentMode::CV) || (currentVentMode == VentMode::CVA))
  {
    DistMotor = mPosVol;
    VelMotor = float(mPosVol / ((TIVal / 1000.0)));
  }
  else if ((currentVentMode == VentMode::CP) || (currentVentMode == VentMode::CPA))
  {
    DistMotor = INITPOSITION;
    VelMotor = float(INITPOSITION / ((TIVal / 1000.0)));
    VelMotor = RISE_TIME * VelMotor;
  }
  else if (currentVentMode == VentMode::CVS)
  {
    DistMotor = mPosVol;
    VelMotor = float(mPosVol / ((TIValAsst / 1000.0)));
  }
  else if (currentVentMode == VentMode::CPS)
  {
    DistMotor = INITPOSITION;
    VelMotor = float(INITPOSITION / ((TIValAsst / 1000.0)));
    VelMotor = RISE_TIME * VelMotor;
  }
  else{
    VelMotor = float(mPosEnd / ((TIVal / 1000.0)));
  }
  AcelMotor = VelMotor * 20;
  SetMotor(DistMotor, VelMotor, AcelMotor);
#endif
}

void functPause(void)
{
  DEBUG("PAUSE");
#ifdef TEST_SENSOR
  pressureUser = pressUser.readCmH2O() - offset1;
  pressPlateau = pressureUser;
  idxPlateau = 0;
  flagPlateau = false;
#endif
}

void functExhale(void)
{
  DEBUG("EXHALE");
  if (currentInput == SMInput::BtnConfig)
  {
    refMotor();
    StatusGraphic=1;
    mPosCurrent = INITPOSITION;
#ifdef TEST_MODE
    if ((PMVal == 0) && (currentVentMode >= VentMode::CP))
    {
      currentVentMode = VentMode::CV;
    }
#endif
    calculeTime(); //solo C/A
#ifdef TEST_SENSOR
    asyncTaskPress.Start();
#endif
#ifdef TEST_LCD
    updateDisplay();
#endif
    storeVarVent();
  }
#ifdef TEST_SENSOR
  pressExhale = pressureUser;
#endif
  DetectRPM();
#ifdef TEST_MOTOR
  calculePositions();
#endif
  //DEBUG("EXHALE1");//30 MILLISECONDS
  asyncTaskTE.Start();
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
