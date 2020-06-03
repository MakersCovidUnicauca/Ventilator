
#include "Definitions.h"

/////////////////////////////////////////
void setup()
{
  while (!Serial)
  {
    ;
  }
  Serial.begin(115200);
  Wire.begin(); // join i2c bus (address optional for master)
  delay(1000);
  DEBUG("Ventilador");
  delay(1000);
  DEBUG("testting");
#ifdef Graphic_Serial
  Serial.println("Fuelle,Paciente ");
#endif

  offset = pressInh.readCmH2O();
  offset1 = pressExh.readCmH2O();

  //Valvulas
  pinMode(VALV_OXIG_PIN, OUTPUT);
  pinMode(VALV_EXTR_PIN, OUTPUT);
  //Boton
  pinMode(BTN_CFG_PIN, INPUT);
  //Motor
  pinMode(STEPPER1_STEP_PIN, OUTPUT);
  pinMode(STEPPER1_DIR_PIN, OUTPUT);
  pinMode(STEPPER1_ENA_PIN, OUTPUT);
  //Hall Sensor
  pinMode(HALL_SENS_PIN, INPUT_PULLUP);
  //default values
  digitalWrite(VALV_OXIG_PIN, LOW);     //cerrar valvula de oxigeno
  digitalWrite(VALV_EXTR_PIN, LOW);     //cerrar valvula de oxigeno
  digitalWrite(STEPPER1_ENA_PIN, HIGH); //cerrar valvula de oxigeno
  InitLCDVent();
  setupGraphics();
  refMotorLCD();
  DEBUG("Serching Motor");
  refMotor(); //busca el sensor optico  solo inicio
  DEBUG("Finish Motor");
  currentState = INIT;
  functInit();
  currentInput = Unknown;
}

void loop()
{

  //update menu
  buttEnc.update();
  if (change == true){
    drawMenu();
    change = false;
  }
  readEncoderUpdate();
  updateMenu();
  //////

  readInput();
  // Tareas maquina de estados
  updateStateMachine();
  //Escanea Boton
  button.update();
  // indica la posicion via Serial
  updateMotorPos();
  //Funcion para actualizacion de pasos del motor
  Motor.run();
  //Funcion para actualizacion de graficas
  loopGraphic();
}

// Actualizacion Tareas de maquina de estados
void readInput()
{
  asyncTask1.Update(asyncTask2);
  asyncTask2.Update(asyncTask3);
  asyncTask3.Update(asyncTask1);
  asyncTask4.Update();
  asyncTask5.Update();
}
