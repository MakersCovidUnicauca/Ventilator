
#ifndef Definitions_h
#define Definitions_h

#include <Arduino.h>
#include <Wire.h>
//Definicion de pines
#include "PinOutVent.h"
//Parametros mecanicos
#include "MechanicalDefinitions.h"
//Modos Ventilador
#include "ModesVentilator.h"
//Websocket
#include "WebsocketVent.h"
//encoder
#include "MenuDefs.h"

//#define Graphic_Serial
#define __DEBG__

#define TEST_SENSOR
#define TEST_MOTOR
//#define TEST_LCD
//#define TEST_MODE
//#define TEST_KEY
#define TEST_ENC


#ifdef __DEBG__
#define DEBUG(a)            \
    Serial.print(millis()); \
    Serial.print(": ");     \
    Serial.println(a);
#else
String TestString;
#define DEBUG(a)  TestString = String(a);
#endif
///////////////////////////////////
//Maquina de estados

#include "AsyncTaskLib.h"
void timeoutTI();
void timeoutTH();
void timeoutTE();
void measurePress();
void waitConfig();

AsyncTask asyncTaskTE(4500, timeoutTE);
AsyncTask asyncTaskTI(1350, timeoutTI);
AsyncTask asyncTaskTH(150, timeoutTH);

AsyncTask asyncTask5(2000, false, waitConfig);

int TimestoSend = 0;
AsyncTask asyncTaskPress(25, true, measurePress);

#define TimeSendGraphic 20

float preUser[TimeSendGraphic];
float volUser[TimeSendGraphic];


#include "ModulesDefs.h"

// Variables globales state machine
SMState currentState;
SMInput currentInput;

/////////////////////////////////// Ventilator
//////////////////////////////////////////////////

// valores por defecto y actualizados por el usuario
float PIPVal = 28;     //Peak inspiratory pressure
float PEEPVal = 6;     //presion residual en el sistema despues de la fase de exhalacion
byte RPMVal = 15;     //respiraciones por minuto
float VOLVal = 500.0; //Volumen total es la cantidad de aire dado al paciente en un ciclo respiratorio
byte IEVal = 2;       //proporcion entre el periodo de inhalacion y el periodo de exhalacion

float VOLRes = 0.0; //Volumen resiudal
byte POVal = 30;    //porcentaje de oxigeno con respecto al VOL,
byte PRVal = 20;    //presion del tanque de oxigeno
byte PMVal = 0;     //Presion maxima que no se puede exceder en los metodos por presion


// ventilador mechanic variables configuradas segun el keypad
#define MDTYPE '1'   // 49 configure the ventilator mode
#define RPMTYPE '2'  //50
#define IETYPE '3'   //51
#define PIPTYPE '4'  //52
#define PEEPTYPE '5' //53
#define POTYPE '6'   //54
#define PRTYPE '7'   //55
#define VOLTYPE '8'  //56

//VentMode currentVentMode = CV;
VentMode tempMode = CV;

//limites inferior y superior de variables
const byte PIPMIN = 1;   //Peak inspiratory pressure Minimun
const byte PIPMAX = 39;  //Peak inspiratory pressure Maximun
const byte PEEPMIN = 0;  //
const byte PEEPMAX = 10; //
const byte RPMMIN = 1;   //
const byte RPMMAX = 40;  //
const byte VOLMIN = 1;   //
const int VOLMAX = 1000; //
const byte IEMIN = 1;    //
const byte IEMAX = 5;    //

const byte POMIN = 21; //porcentaje de oxigeno con respecto al VOL,
const byte POMAX = 90; //

const byte PRMIN = 1;  //presion del regulador de el tanque de oxigeno,
const byte PRMAX = 55; //

const byte PM_FAB = 60;  //Presion maxima que no se puede exceder en los metodos por presion
//const byte PLI_FAB = 40; //Presion Limite Inhalacion

// valores calculados en milliseconds
#define TIME_OXY_AIR 700    //30 RPM
#define RISE_TIME   1

uint16_t TVal = 0;  //The length of time (in seconds) of an inhale/exhale cycle
uint16_t TIVal = 0; //The length of time (in seconds) of the inspiratory phase
uint16_t TI = 0;
uint16_t TEVal = 0;   //The length of time (in seconds) of the expiratory phase
uint16_t THVal = 200; // 0.2 second The amount of time (in seconds) to hold the compression at the end of the inhale for plateau pressure

float VIVal = 0; // The rotation rate of the inspiratory phase (in pulses/second)
float VEVal = 0; // The velocity of the fingers in the expiratory phase (in pulses/second).

#define TIMESEC 60000

// variables que almacenan el tipo de variable y el valor
byte valType = 0;

/////////////////////////////////// Pressure
//////////////////////////////////////////////////
#include <Pressure.h>

Pressure pressAmbu(PRESS_AMBU_PIN);
Pressure pressUser(PRESS_USR_PIN);
Pressure pressOxig(PRESS_OXIG_PIN);
float offset, offset1;

float pressureUser = 0;
float pAmbu = 0;
float pressInhale = 2.3;
float pressExhale = -3.45;
float pressPlateau = 4.76;
byte idxPlateau = 0;
float pressOxygen;

bool FlagOxig = false;
bool FlagAire = false;
bool FlagPressure = false;

#define INTERVAL 20000    //20 millisecond
#define SENS_PRESS 2    //1-2 cmH2O
unsigned long prevMicros;
bool flagTime = false;

// Motor
void refMotor();
void InitMotor();
void SetMotor(float Distance, float speedM, float accel);
float GetPosition();
// State Machine
void updateMotorPos();
void functInit();
void updateStateMachine();

//Lcd
void InitLCDVent();
void updateDisplay();
void updateDisplayPressure();
void refMotorLCD();
void mainMsg();

void readKey();
bool checkVal(byte valType, int valValue);
String checkType(byte valType);

void buttonChanged(int state);
void storeVarVent();
void readVarVent();
void measurePress();

//Graficas
void loopGraphic();
void setupGraphics();

#endif