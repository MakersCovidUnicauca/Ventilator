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
  // Busqueda posicion de inicio
  digitalWrite(STEPPER1_ENA_PIN,LOW); //

  #ifdef InitSensorEnable
  SetMotor(refDistance, refSpeed, refAccel);
  while(digitalRead(HALL_SENS_PIN)!= LOW){
   Motor.run();
   delay(10); 
  }
  #else
  digitalWrite(STEPPER1_ENA_PIN,HIGH); //
  delay(1000);
  while(digitalRead(HALL_SENS_PIN)!= HIGH){
   delay(200); 
   Serial.println("waiting position ");
  }
  #endif

  digitalWrite(STEPPER1_ENA_PIN,LOW); //
  #ifdef __DEBG__
   Serial.print("Refered position ");
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

float GetPosition(){
  float positionMotor = 0;
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
* Initialize Motor at initial position
* *----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void InitMotor(){
  Motor.setCurrentPosition(INITPOSITION);
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
void SetMotor(float Distance, float speedM, float accel){
  long DistanceValue = (long)Distance * PulseXmm;
  Motor.setMaxSpeed(speedM *  PulseXmm);
  Motor.setAcceleration(accel * PulseXmm);
  Motor.moveTo(-DistanceValue);
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
  int dist = (int)GetPosition();
  if(mPosCurrent != dist){
    String stringone = "Dist:";
    stringone = stringone + String(dist);
    mPosCurrent = dist;
    //DEBUG(stringone);
  }
}
