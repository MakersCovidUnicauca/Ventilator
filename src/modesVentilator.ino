#include "Definitions.h"

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

void CtrlPP()
{
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
                currentInput = SMInput::PPCtrl;
                flagTime = false;
            //    prevMicros = micros();
            }
        }
    }
}

void MngAssitExh()
{
  if ((currentVentMode == VentMode::CVA) || (currentVentMode == VentMode::CPA))
  {
    pressureUser = pressUser.readCmH2O() - offset1;
    if (pressureUser < pressExhale)
    {
        pressExhale = pressureUser;
    }
    if (pressExhale >= (PEEPVal - SENS_PRESS))
    {
        DEBUG("PLI_DETEC");
        asyncTaskTE.Stop();
        asyncTaskTI.Start();
        currentInput = SMInput::TEEnd;
    }
  }
}


void MngAssitInh()
{
  if ((currentVentMode == VentMode::CVA) || (currentVentMode == VentMode::CPA))
  {
    pressureUser = pressUser.readCmH2O() - offset1;
    if (pressureUser > pressInhale)
    {
      pressInhale = pressureUser;
    }
    if (pressInhale >= (PIPVal - SENS_PRESS))
    {
        DEBUG("TI_END");
        asyncTaskTI.Stop();
        asyncTaskTH.Start();
        currentInput = SMInput::TIEnd;
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

