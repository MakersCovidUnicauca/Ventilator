/*!
\file   MotorVent.ino
\date   2020-05-05
\author Fulvio Vivas <fulvio.vivas@gmail.com>
\brief  Functions Motor.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of XXXXXX, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) XXXXX 2020. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/


/*F**************************************************************************
* NAME: refMotor
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Search Motor Reference Position
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
#include "Definitions.h"


#define refDistance  70.0
#define refSpeed  20.0
#define refAccel  20.0

void refMotor(){
  // Posicion y velocidades de busqueda
  SetMotor(refDistance, refSpeed, refAccel);
  digitalWrite(STEPPER1_ENA_PIN,HIGH); //
  #ifdef __DEBG__
   Serial.print("Ref position ");
   Serial.println(GetPosition());
   #endif
  while(digitalRead(HALL_SENS_PIN)!= HIGH){
   Motor.run();
   
   delay(20); 
  }
  digitalWrite(STEPPER1_ENA_PIN,LOW); //
  #ifdef __DEBG__
   Serial.print("Ref position ");
   Serial.println(GetPosition());
   #endif
}

/*F**************************************************************************
* NAME: GetPosition
*----------------------------------------------------------------------------
* PARAMS:
* paramt: none
* return: Current Position of Motor
* *----------------------------------------------------------------------------
* PURPOSE:
* Get Motor Position
*----------------------------------------------------------------------------
* NOTE:
* Monitor Current Position in the Ventilator Operation
*****************************************************************************/

long GetPosition(){
  long positionMotor = 0;
  positionMotor = -Motor.currentPosition()/PulseXmm;
  return positionMotor;
}

/*F**************************************************************************
* NAME: InitMotor
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Initialize Motor at Zero Position
* *----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void InitMotor(){
  Motor.setCurrentPosition(0);
}

/*F**************************************************************************
* NAME: SetMotor
*----------------------------------------------------------------------------
* PARAMS:
* Distance: Motor Target Position
* speedM: Motor Speed
* accel: Motor Acceleration
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Set Motor Configuration Parameters
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void SetMotor(long Distance, float speedM, float accel){
  //Motor.setSpeed(speedM *  PulseXmm);
  Motor.setMaxSpeed(speedM *  PulseXmm);
  Motor.setAcceleration(accel * PulseXmm);
  Motor.moveTo(-Distance * PulseXmm);
}

/*F**************************************************************************
* NAME: updateMotorPos
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Update Motor Position
* *----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void updateMotorPos(){
  int dist = GetPosition();
  if(mPosCurrent != dist){
    String stringone = "Dist:";
    stringone = stringone + String(dist);
    mPosCurrent = dist;
    DEBUG(stringone);
  }
}
