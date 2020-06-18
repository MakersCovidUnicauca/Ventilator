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
                mPosCurrent
                currentInput = SMInput::PICtrl;
                flagTime = false;
            //    prevMicros = micros();
            }
        }
    }
}

void CtrlPEEP()
{
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
    if (pressureUser < pressExhale)
    {
        pressExhale = pressureUser;
    }
    if (pressExhale >= PLI_FAB)
    {
        DEBUG("PLI_DETEC");
        asyncTaskTE.Stop();
        asyncTaskTI.Start();
        currentInput = SMInput::TEEnd;
    }
}

void MngAssitInh()
{
    if (pressureUser > pressInhale)
    {
      pressInhale = pressureUser;
    }
    if (pressInhale >= PLI_FAB)
    {
        DEBUG("TI_END");
        asyncTaskTI.Stop();
        asyncTaskTH.Start();
        currentInput = SMInput::TIEnd;
    }
}
