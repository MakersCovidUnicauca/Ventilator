////////////////maquinas de estado//////////////////////
//

#include "Definitions.h"

////Simulation sin btnConfig
void waitConfig(){
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
 
  float pInh = pressInh.readCmH2O()-offset;
  pressureTemp = pressExh.readCmH2O()-offset1;
  float pOxig = 0.0;

  if(FlagOxig == true){
    pOxig = pressOxig.readCmH2O();
    if(pOxig < PRVal){
      //alarma(AlarmType::OXY);
    }
  }
  FlagPressure = true;

#ifdef TEST_LCD
  updateDisplayPressure();
#endif

// 
TimestoPrint++;
if(TimestoPrint >= TimestoPrintSerial ){
  TimestoPrint = 0;
    #ifdef __DEBG__
      DEBUG("PRESSURE");
      Serial.print("PInh: ");
      Serial.print(pInh,3);
      Serial.print(", PExh: ");
      Serial.print(pressureTemp,3);
      Serial.print(", POxig: ");
      Serial.println(pOxig,3);
  #endif
  #ifdef  Graphic_Serial
    Serial.print(pInh,3);
    Serial.print(",");
    Serial.print(pressureTemp,3);
    Serial.println(" ");
  #endif
}
}

void CtrlPressure(){
  if(currentVentMode == VentMode::CP){
    float pInh = pressInh.readCmH2O();
    if(pInh >= PM_FAB){
      DEBUG("PM_DETEC");
      Motor.stop();
      DistMotor = mPosInit;
      VelMotor = 100.0;
      AcelMotor = 100.07;
      SetMotor(DistMotor, VelMotor, AcelMotor);
    }
  }
}

void MngAssitExh(){
  if(FlagPressure){
    if(pressureTemp < pressExhale){
      pressExhale = pressureTemp;
    }
    if((currentVentMode == VentMode::CVA) || (currentVentMode == VentMode::CPA)){
      //pressExhale = pressExh.readCmH2O();
      //if((mPosCurrent == mPosEnd) && (FlagAire == true)){
      if(pressExhale >= PLI_FAB){
        DEBUG("PLI_DETEC");
        asyncTask1.Stop();
        asyncTask2.Start();
        currentInput = SMInput::TEEnd;
      }
    }
    FlagPressure = false;
  }
}

void MngAssitInh(){
  if(FlagPressure){
    if(pressureTemp > pressInhale){
      pressInhale = pressureTemp;
    }
    if((currentVentMode == VentMode::CVA) || (currentVentMode == VentMode::CPA)){
      //pressInhale = pressExh.readCmH2O();
      if(pressInhale >= PLI_FAB){
        DEBUG("TI_END");
        asyncTask2.Stop();
        asyncTask3.Start();
        currentInput = SMInput::TIEnd;        
      }
    }
    FlagPressure = false;
  }
}

void calculePlateau(){
  if(FlagPressure){
    //float pExh = pressExh.readCmH2O();
    if(pressureTemp != pressPlateau){
       pressPlateau = pressureTemp;
    }
    else{
      idxPlateau++;
    }
    if(idxPlateau == 4){
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
  TIVal = TVal/(1+IEVal);
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

void alarma(byte alarm){
  String stringAlarm = "ALARMA:";
  String stringone;
  switch (alarm)
  {
    case PWR: stringone = "POWER"; break;
    case OXY: stringone = "OXYGENO"; break;
    case ALM: stringone = "ALARMA:"; break;
    case TEST: stringone = "TEST:"; break;
  }
  stringAlarm = stringAlarm + stringone;
  DEBUG(stringAlarm);
}

void calculeVol(){
  if((Motor.isRunning() == 0) && (FlagAire == false)){
    DEBUG("AIRE");
    //cerrar valvula de oxigeno
    digitalWrite(VALV_OXIG_PIN,LOW);
    FlagOxig = false;
    FlagAire = true;
    mPosEnd = float(VOLVal/RELMMVOL);
    DistMotor = INITPOSITION - mPosEnd; 
    // de la posicion final debe regresarse para cargar el volumen requerido
    float Po = POVal*0.01;   //
    VelMotor = float(mPosEnd/((TIVal*(1-Po))/1000.0)); 
    AcelMotor = VelMotor * 30;
    SetMotor(DistMotor, VelMotor, AcelMotor);
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
    Serial.print(VelMotor,3);
    Serial.print(" AccelM: ");
    Serial.println(AcelMotor,3);
#endif
  }
}

void calculeOxig(){
  DEBUG("OXIGENO");
  VOLRes = (mPosCurrent - mPosInit)* RELMMVOL;
  VOLRes = 0;
  float Vol = VOLVal - VOLRes;
  float Po = POVal*0.01;   //
  //mPosOxi = float((Vol*Po)/RELMMVOL);
  mPosOxi = VOLVal - float(((1- Po)*VOLVal/0.79));
  mPosOxi =  (mPosOxi/RELMMVOL);
  //abrir valvula de oxigeno
  digitalWrite(VALV_OXIG_PIN,HIGH);
  FlagOxig = true;
  DistMotor = INITPOSITION - mPosOxi;
  VelMotor = mPosOxi/((TIVal*Po)/1000.0);
  AcelMotor = VelMotor*30;
  SetMotor(DistMotor, VelMotor, AcelMotor);
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
  Serial.print(VelMotor,3);
  Serial.print(" AccelM: ");
  Serial.println(AcelMotor,3);
#endif

}


void stateInit()
{
  if (currentInput == SMInput::BtnReset)
    changeState(SMState::CONFIG);
  if (currentInput == SMInput::BtnConfig) //Simulation sin btnConfig
    changeState(SMState::EXHALE); ////Simulation sin btnConfig
}

// Acciones de los estados y condiciones de transiciones
void stateConfig()
{
  if (currentInput == SMInput::BtnConfig)
    changeState(SMState::EXHALE);
  readKey();
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
  calculeVol();
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

void functInit(void){
  DEBUG("INIT");
  
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("Ventilador ");

  button.setCallback(buttonChanged);
  asyncTask5.Start(); ////Simulation sin btnConfig
  //readVarVent();  ////Simulation sin btnConfig
  
}

void functConfig(void){
  DEBUG("CONFIG");
  readVarVent();
}

void functInhale(void){
  DEBUG("INHALE");
  #ifdef TEST_SENSOR
  pressInhale = pressureTemp;
  #endif
  #ifdef TEST_MOTOR
  FlagAire = false;

  DistMotor = INITPOSITION;
  VelMotor =  float(mPosEnd/((TIVal/1000.0))); ;
  AcelMotor = VelMotor * 30;
  SetMotor(DistMotor, VelMotor, AcelMotor);
  #endif  
}

void functPause(void){
  DEBUG("PAUSE");
  #ifdef TEST_SENSOR
  pressPlateau = pressureTemp;
  idxPlateau = 0;
  #endif
}

void functExhale(void){
  DEBUG("EXHALE");
  if (currentInput == SMInput::BtnConfig){
    #ifdef TEST_MODE
    if((PMVal == 0) && (currentVentMode >= VentMode::CP)){
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
  pressExhale = pressureTemp;
  #endif
  #ifdef TEST_MOTOR
  calculeOxig();
  #endif
  //DEBUG("EXHALE1");//30 MILLISECONDS
  asyncTask1.Start();
}

void functListen(void){
  DEBUG("LISTEN");
}

void functExhPas(void){
  DEBUG("EXHALE PAUSE");
}

void functDefault(void){
  DEBUG("ERROR STATE");
}

// Actualiza el estado de la maquina
void updateStateMachine()
{
  switch (currentState)
  {
    case INIT: stateInit(); break;
    case CONFIG: stateConfig(); break;
    case INHALE: stateInhale(); break;
    case PAUSE: statePause(); break;
    case EXHALE: stateExhale(); break;
    case LISTEN: stateListen(); break;
    case EXLPAS: stateExlPas(); break;
  }
}


// Funcion que cambia el estado y dispara las transiciones
void changeState(int newState)
{
  currentState = (SMState)newState;

  switch(currentState){
      case SMState::INIT: // state init
          functInit();
      break;
      case SMState::CONFIG: // state configuration
          functConfig();
      break;
      case SMState::INHALE:  // state inhale
          functInhale();
      break;
      case SMState::PAUSE:  // state exhale
          functPause();
      break;
      case SMState::EXHALE:  // state pause
          functExhale();
      break;
      case SMState::LISTEN:  // state listen
          functListen();
      break;
      case SMState::EXLPAS:  // state exhale pause
          functExhPas();
      break;
      default:
          functDefault();
      break;
  }
}
