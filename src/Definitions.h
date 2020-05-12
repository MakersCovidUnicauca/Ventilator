
#ifndef Definitions_h
#define Definitions_h

#include <Arduino.h>

#include <Wire.h> 
#include "PinOutVent.h"

//#define Graphic_Serial
#define __DEBG__

#define TEST_SENSOR
//#define TEST_MOTOR
//#define TEST_LCD
//#define TEST_MODE


#ifdef __DEBG__
#define DEBUG(a) Serial.print(millis()); Serial.print(": "); Serial.println(a);
#else
String stringDebug;
#define DEBUG(a) stringDebug = a;
#endif
///////////////////////////////////
//////////////////////////////////////////////////

#include "AsyncTaskLib.h"

void timeoutTI();
void timeoutTH();
void timeoutTE();
void measurePress();

AsyncTask asyncTask1(4500,timeoutTE);
AsyncTask asyncTask2(1350,timeoutTI);
AsyncTask asyncTask3(150,timeoutTH);

#ifdef __DEBG__
AsyncTask asyncTask4(50, true, measurePress);
#endif

#ifdef Graphic_Serial
AsyncTask asyncTask4(20, true, measurePress);
#endif

/////////////////////////////////// Keypad
//////////////////////////////////////////////////

#include "Keypad.h"

const byte ROWS = 4; 
const byte COLS = 3; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {KEY_R1_PIN, KEY_R2_PIN, KEY_R3_PIN, KEY_R4_PIN}; 
byte colPins[COLS] = { KEY_C1_PIN, KEY_C2_PIN, KEY_C3_PIN};

int stateKey = 0;
int pos = 0;
char inputCode[4] = {'1', '0', '3', '0'};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

/////////////////////////////////// Button
//////////////////////////////////////////////////
#include <ButtonDebounce.h>
ButtonDebounce button(BTN_CFG_PIN, 250); //HIGH = config LOW = Normal
void buttonChanged(int state);

/////////////////////////////////// LiquidCrystal
//////////////////////////////////////////////////

#include <LiquidCrystal_PCF8574.h>
LiquidCrystal_PCF8574 lcd(0x27);  //Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
/////////////////////////////////// State machine
//////////////////////////////////////////////////
enum SMState
{
  INIT,
  CONFIG,
  INHALE,
  PAUSE,
  EXHALE,
  LISTEN,
  EXLPAS
};

enum SMInput
{
  Unknown,
  BtnConfig,
  TIEnd,
  THEnd,
  TEEnd,
  TLEnd,
  TPEnd,
  BtnReset
};

// Variables globales state machine
SMState currentState;
SMInput currentInput;

///////////////////////////////////   Stepper Motor
//////////////////////////////////////////////////

#include <AccelStepper.h>
// The X Stepper pins
#define PulsePerRev 3200
#define mmPerRev 60
#define mmPerPulse  0.01875
#define PulseXmm 53

#define DelayPulse 220 // millisecond
#define LONG_FUELLE 60 //milimetros (mm)
#define RELMMVOL 17.67

float mPosInit = 0; // mm
float mPosEnd = LONG_FUELLE; // mm
float mPosCurrent;  // mm
float mPosOxi = 0;  // mm

//diametro 15 cm
//area 176 cm2
//1.76 ml * ml

#define DIST_MOTOR     60  // mm
#define VEL_MOTOR     30.0  // mm/s
#define ACCEL_MOTOR   30.0  // mm/s2

long DistMotor = DIST_MOTOR;
float VelMotor = VEL_MOTOR;  // mm/s
float AcelMotor = ACCEL_MOTOR;  // mm/s2

AccelStepper Motor(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);

/////////////////////////////////// Ventilator
//////////////////////////////////////////////////

// ventilador mechanic variables configuradas segun el keypad
#define MDTYPE  '1'  // 49 configure the ventilator mode
#define RPMTYPE '2' //50
#define IETYPE  '3' //51
#define PIPTYPE '4' //52
#define PEEPTYPE '5'  //53
#define POTYPE '6'  //54
#define PRTYPE '7'  //55
#define VOLTYPE '8' //56

enum VentMode
{
  CV, //CONTROL POR VOLUMEN
  CVA,  //CONTROL FOR VOLUMEN ASISTIDO
  CP, //CONTROL POR PRESION
  CPA  //CONTROL POR PRESION ASISTIDA
};

VentMode currentVentMode = CV;
VentMode tempMode  = CV;

enum AlarmType
{
  PWR, //CONTROL POR VOLUMEN
  OXY,  //CONTROL FOR VOLUMEN ASISTIDO
  ALM, //CONTROL POR PRESION
  TEST  //CONTROL POR PRESION ASISTIDA
};


//limites inferior y superior de variables
const byte PIPMIN = 1; //Peak inspiratory pressure Minimun
const byte PIPMAX = 39; //Peak inspiratory pressure Maximun
const byte PEEPMIN = 0; //
const byte PEEPMAX = 10; //
const byte RPMMIN = 1; //
const byte RPMMAX = 40; //
const byte VOLMIN = 1; //
const int VOLMAX = 1000; //
const byte IEMIN = 1; //
const byte IEMAX = 5; //

const byte POMIN = 21; //porcentaje de oxigeno con respecto al VOL,
const byte POMAX = 90; //

const byte PRMIN = 1; //presion del regulador de el tanque de oxigeno,
const byte PRMAX = 55; //

const byte PM_FAB = 60; //Presion maxima que no se puede exceder en los metodos por presion
const byte PLI_FAB = 40; //Presion Limite Inhalacion

// valores por defecto y actualizados por el usuario
byte PIPVal = 28; //Peak inspiratory pressure
byte PEEPVal = 6; //presion residual en el sistema despues de la fase de exhalacion
byte RPMVal = 5; //respiraciones por minuto
float VOLVal = 500.0; //Volumen total es la cantidad de aire dado al paciente en un ciclo respiratorio
byte IEVal = 2; //proporcion entre el periodo de inhalacion y el periodo de exhalacion

float VOLRes = 0.0;
byte POVal = 30;  //porcentaje de oxigeno con respecto al VOL,
byte PRVal = 20; //presion del tanque de oxigeno
byte PMVal = 0; //Presion maxima que no se puede exceder en los metodos por presion

// valores calculados en milliseconds
uint16_t TVal = 0;  //The length of time (in seconds) of an inhale/exhale cycle
uint16_t TIVal = 0; //The length of time (in seconds) of the inspiratory phase
uint16_t TI = 0;
uint16_t TEVal = 0; //The length of time (in seconds) of the expiratory phase
uint16_t THVal = 200; // 0.2 second The amount of time (in seconds) to hold the compression at the end of the inhale for plateau pressure

float VIVal = 0; // The rotation rate of the inspiratory phase (in pulses/second)
float VEVal = 0; // The velocity of the fingers in the expiratory phase (in pulses/second).

#define TIMESEC 60000

// variables que almacenan el tipo de variable y el valor
byte valType = 0; 

/////////////////////////////////// Pressure
//////////////////////////////////////////////////
#include <Pressure.h>

Pressure pressInh(PRESS_AMBU_PIN);
Pressure pressExh(PRESS_USR_PIN);
Pressure pressOxig(PRESS_OXIG_PIN);
float offset,offset1;

float pressureTemp = 0;
float pressInhale = 2.3;
float pressExhale = -3.45;
float pressPlateau = 4.76;
byte idxPlateau = 0;
float pressOxygen;

bool FlagOxig = false;
bool FlagAire = false;
bool FlagPressure = false;

/////////////////// EEPROM
#include <EEPROM.h>
/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int addrVarVent = 0x30;

#define MRK_STR_VAR 0xAA  // Mark Store EEprom variable ventilator

// Motor
void refMotor();
void InitMotor();
void SetMotor(long Distance, float speedM, float accel);
// State Machine
void updateMotorPos();
void functInit();
void updateStateMachine();

//Lcd
void  updateDisplay();
void updateDisplayPressure();

void readKey();
bool checkVal(byte valType, int valValue);
String checkType(byte valType);

void buttonChanged(int state);
void storeVarVent();
void readVarVent();
void measurePress();

#endif