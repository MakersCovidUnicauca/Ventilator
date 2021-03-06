#include "Definitions.h"

/*
// Funcion que cambia el estado y dispara las transiciones
void readInputModeVent()
{
  //currentVentMode = (VentMode)newState;
  switch (currentVentMode)
  {
  case VentMode::CV: //CONTROL POR VOLUMEN
    asyncTaskTE.Update(asyncTaskTI);
    asyncTaskTI.Update(asyncTaskTH);
    asyncTaskTH.Update(asyncTaskTE);
    break;
  case VentMode::CVA: //CONTROL FOR VOLUMEN ASISTIDO
    functConfig();
    break;
  case VentMode::CP: //CONTROL POR PRESION
    functInhale();
    break;
  case VentMode::CPA: //CONTROL POR PRESION ASISTIDA
    functPause();
    break;
  case VentMode::CVS: //CONTROL FOR VOLUMEN ESPONTANEO
    functExhale();
    break;
  case VentMode::CPS: //CONTROL POR PRESION ESPONTANEO
    functListen();
    break;
  case VentMode::CPAP: //CONTROL POR PRESION POSITIVA CONTINUA EN LA VIA AEREA
    functExhPas();
    break;
  case VentMode::SIMV: //CONTROL FOR VOLUMEN ASISTIDO
    functExhPas();
    break;
  default:
    functDefault();
    break;
  }
}
*/

void CtrlPIP()
{
  if ((currentVentMode == VentMode::CP) || (currentVentMode == VentMode::CPA))
  {
    pressureUser = pressUser.readCmH2O() - offset1;
    if (pressureUser >= PIPVal)
    {
        if(flagTime == false){
          prevMicros = micros();
          flagTime = true;
        }
        else{
          unsigned long currentMicros = micros();
          if ((unsigned long)(currentMicros - prevMicros) >= INTERVAL)
          {
            DEBUG("PIP_DETEC");
            Motor.stop();
            currentInput = SMInput::PICtrl;
            flagTime = false;
            //    prevMicros = micros();
          }
        }
    }
  }
}

void CtrlVol()
{
  if ((currentVentMode == VentMode::CV) || (currentVentMode == VentMode::CVA))
  {
    if (mPosCurrent >= mPosVol)
    {
        if(flagTime == false){
          prevMicros = micros();
          flagTime = true;
        }
        else{
          unsigned long currentMicros = micros();
          if ((unsigned long)(currentMicros - prevMicros) >= INTERVAL)
          {
            DEBUG("VOL_DETEC");
            Motor.stop();
            currentInput = SMInput::VolCtrl;
            flagTime = false;
            //    prevMicros = micros();
          }
        }
    }
  }
}

/*
void CtrlPEEP()
{
    pressureUser = pressUser.readCmH2O() - offset1;
    if (pressureUser < (PEEPVal - SENS_PRESS))
    {
        if(flagTime == false){
            prevMicros = micros();
            flagTime = true;
        }
        else{
            unsigned long currentMicros = micros();
            if ((unsigned long)(currentMicros - prevMicros) >= INTERVAL)
            {
                DEBUG("PEEP_DETEC");
                currentInput = SMInput::PECtrl;
                flagTime = false;
            //    prevMicros = micros();
            }
        }
    }
}
*/

void CtrlPP()
{
  if ((currentVentMode == VentMode::CVS) || (currentVentMode == VentMode::CPS))
  {
    if(flagPlateau == true){
      pressureUser = pressUser.readCmH2O() - offset1;
      if (pressureUser < (pressPlateau - SENS_PRESS))
      {
          if(flagTime == false){
              prevMicros = micros();
              flagTime = true;
          }
          else{
              unsigned long currentMicros = micros();
              if ((unsigned long)(currentMicros - prevMicros) >= INTERVAL)
              {
                  DEBUG("PP_DETEC");
                  asyncTaskTH.Stop();
                  THVal = asyncTaskTH.GetElapsedTime() + THVal;
                  asyncTaskTE.Start();
                  currentInput = SMInput::PPCtrl;
                  currentInput = SMInput::THEnd;
                  flagTime = false;
                  flagPlateau = false;
              }
          }
      }
    }
  }
}


void MngAssitExh()
{
  if ((currentVentMode == VentMode::CVA) || (currentVentMode == VentMode::CPA))
  {
    pressureUser = pressUser.readCmH2O() - offset1;
    if (pressureUser <= (PEEPVal - SENS_PRESS))
    {
      if(flagTime == false){
        prevMicros = micros();
        flagTime = true;
      }
      else{
        unsigned long currentMicros = micros();
        if ((unsigned long)(currentMicros - prevMicros) >= INTERVAL)
        {
          DEBUG("ASSIT_EXH");
          if(!((FlagOxig == false) && (FlagAire == false))){
            Motor.stop();
          }
          asyncTaskTE.Stop();
          asyncTaskTI.Start();
          currentInput = SMInput::TEEnd;
          flagTime = false;
         }
      }
    }
  }
}

void MngEspExh()
{
  if ((currentVentMode == VentMode::CVS) || (currentVentMode == VentMode::CPS))
  {
    pressureUser = pressUser.readCmH2O() - offset1;
    if (pressureUser <= (PEEPVal - SENS_PRESS))
    {
      if(flagTime == false){
        prevMicros = micros();
        flagTime = true;
      }
      else{
        unsigned long currentMicros = micros();
        if ((unsigned long)(currentMicros - prevMicros) >= INTERVAL)
        {
          DEBUG("ESP_EXH");
          if(!((FlagOxig == false) && (FlagAire == false))){
            Motor.stop();
          }
          asyncTaskTE.Stop();
          TEVal = asyncTaskTE.GetElapsedTime() + TEVal;
          asyncTaskTI.Start();
          currentInput = SMInput::TEEnd;
          flagTime = false;
        }
      }
    }
  }
}

void calculePressIns()
{
  if (FlagPressure)
  {
    pressureUser = pressUser.readCmH2O() - offset1;
    if (pressureUser >= pressInhale)
    {
      pressInhale = pressureUser;
    }
    FlagPressure = false;
  }
}


void MngAssitInh()
{
  if ((currentVentMode == VentMode::CVA) || (currentVentMode == VentMode::CPA))
  {
    pressureUser = pressUser.readCmH2O() - offset1;
    if (pressureUser <= (pressInhale - SENS_PRESS))
    {
      if(flagTime == false){
        prevMicros = micros();
        flagTime = true;
      }
      else{
        unsigned long currentMicros = micros();
        if ((unsigned long)(currentMicros - prevMicros) >= INTERVAL)
        {
          DEBUG("ASSIT_INH");
          if(!((currentInput == SMInput::VolCtrl) || (currentInput == SMInput::PICtrl))){
            Motor.stop();
          }
          asyncTaskTI.Stop();
          //TIVal = asyncTaskTI.GetElapsedTime();
          asyncTaskTH.Start();
          currentInput = SMInput::TIEnd;
          flagTime = false;
        //    prevMicros = micros();
        }
      }
    }
  }
}

//if (mPosCurrent >= mPosVol)
//pressureUser = pressUser.readCmH2O() - offset1;
//    if (pressureUser >= PIPVal)
void DetectRPM(){
  if ((currentVentMode == VentMode::CVS) || (currentVentMode == VentMode::CPS))
  {
     if(idxDetect > 10){
      idxDetect = 0;
      THValAsst = THVal/10;
      TEValAsst = TEVal/10;   //The length of time (in seconds) of the expiratory phase
      TIAsst = TI/10;
      TIValAsst = THValAsst + TIAsst;
      TValAsst = TIValAsst + TEValAsst; //The length of time (in seconds) of an inhale/exhale cycle
      RPMValAsst = (TIMESEC/TValAsst);
        
      TVal = 0;  //The length of time (in seconds) of an inhale/exhale cycle
      TIVal = 0; //The length of time (in seconds) of the inspiratory phase
      TI = 0;
      TEVal = 0;   //The length of time (in seconds) of the expiratory phase
      THVal = 0; // 0.2 second The amount of time (in seconds) to hold the compression at the end of the inhale for plateau pressure
    
#ifdef __DEBG__
  Serial.print("TAsst: ");
  Serial.print(TValAsst);
  Serial.print(" TIAsst: ");
  Serial.print(TIAsst);
  Serial.print(" TPAsst: ");
  Serial.print(THValAsst);
  Serial.print(" TEAsst: ");
  Serial.println(TEValAsst);
#endif
    
    }
    idxDetect++;
  }
}


void MngEspInh()
{
  if ((currentVentMode == VentMode::CVS) || (currentVentMode == VentMode::CPS))
  {
    if(currentVentMode == VentMode::CVS){
      pressureUser = pressUser.readCmH2O() - offset1;
      if (pressureUser <= (pressInhale - SENS_PRESS))
      {
        flagTrigger = true;
      }
    }
    else if(currentVentMode == VentMode::CVS){
      if (mPosCurrent >= mPosVol){
        flagTrigger = true;
      }
    }
    else{
      flagTrigger = false;
    }

    if(flagTrigger == true){

      if(flagTime == false){
        prevMicros = micros();
        flagTime = true;
      }
      else{
        unsigned long currentMicros = micros();
        if ((unsigned long)(currentMicros - prevMicros) >= INTERVAL)
        {
          DEBUG("ESP_INH");
          if(!((currentInput == SMInput::VolCtrl) || (currentInput == SMInput::PICtrl))){
            Motor.stop();
          }
          asyncTaskTI.Stop();
          TIVal = asyncTaskTI.GetElapsedTime() + TIVal;
          asyncTaskTH.Start();
          currentInput = SMInput::TIEnd;
          flagTime = false;
        }
      }
      flagTrigger = false;
    }
  }
}


void functInhaleCV(void)
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

void functExhaleCV(void)
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
    calculeTime();
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
#ifdef TEST_MOTOR
  calculePositions();
#endif
  //DEBUG("EXHALE1");//30 MILLISECONDS
  asyncTaskTE.Start();
}

