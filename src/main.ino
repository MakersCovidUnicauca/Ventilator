
#include "Definitions.h"



/////////////////////////////////////////
void setup(){
  while (!Serial) {
    ;
    }
  Serial.begin (115200);
  Wire.begin();        // join i2c bus (address optional for master)
  delay(1000);
  Serial.println ("Ventilador");
  delay(1000);
  Serial.print("testting");

  offset = pressInh.readCmH2O();
  offset1 = pressExh.readCmH2O();

//Valvulas
    pinMode(VALV_OXIG_PIN,OUTPUT);
    pinMode(VALV_EXTR_PIN,OUTPUT);
//Boton
    pinMode(BTN_CFG_PIN,INPUT);
//Motor
    pinMode(STEPPER1_STEP_PIN,OUTPUT);
    pinMode(STEPPER1_DIR_PIN,OUTPUT);
    pinMode(STEPPER1_ENA_PIN,OUTPUT);
//Hall Sensor
    pinMode(HALL_SENS_PIN,INPUT_PULLUP);
//default values  
    digitalWrite(VALV_OXIG_PIN,LOW); //cerrar valvula de oxigeno
    digitalWrite(VALV_EXTR_PIN,LOW); //cerrar valvula de oxigeno
    digitalWrite(STEPPER1_ENA_PIN,HIGH); //cerrar valvula de oxigeno
  Serial.println ("Serching Motor");
  refMotor(); //busca el sensor de hall solo inicio
  InitMotor(); // solo inicio
  updateMotorPos();
  Serial.println ("Finish Motor");
  
  currentState = INIT;
  functInit();
  currentInput = Unknown;
  
}
  
void loop(){

  readInput();
  // Tareas maquina de estados
  updateStateMachine();
  //Escanea Boton 
  button.update();
  // indica la posicion via Serial 
  updateMotorPos();
  //Funcion para actualizacion de pasos del motor
  Motor.run();
}

// Actualizacion Tareas de maquina de estados
void readInput()
{
  asyncTask1.Update(asyncTask2);
  asyncTask2.Update(asyncTask3);
  asyncTask3.Update(asyncTask1);
  asyncTask4.Update();
}
