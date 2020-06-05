

#include "Definitions.h"
#include <PID_v1.h>

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Define the aggressive and conservative Tuning Parameters
double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

void InitPID(){

  Setpoint = PIPVal;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

}

void CtrlPressurePID()
{

//Pressure pressInh(PRESS_AMBU_PIN);
//Pressure pressExh(PRESS_USR_PIN);
  DEBUG("PM_DETEC");

  if (currentVentMode == VentMode::CP)
  {
    Input = pAmbu;

    double gap = abs(Setpoint-Input); //distance away from setpoint
    if (gap < 10)
    {  //we're close to setpoint, use conservative tuning parameters
        myPID.SetTunings(consKp, consKi, consKd);
    }
    else
    {
     //we're far from setpoint, use aggressive tuning parameters
        myPID.SetTunings(aggKp, aggKi, aggKd);
    }
    
    myPID.Compute();
  
    VelMotor = Output;
    Motor.setMaxSpeed(VelMotor * PulseXmm);
   
  }
}