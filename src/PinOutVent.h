//﻿#define __Other__
//#define Arduino_PCB
#ifndef PINOUT_BOARDS
#define PINOUT_BOARDS

#define ESP32_PCB

#ifdef ESP32_PCB
// Teclado
#define KEY_R1_PIN 13
#define KEY_R2_PIN 12
#define KEY_R3_PIN 14
#define KEY_R4_PIN 27
#define KEY_C1_PIN 26
#define KEY_C2_PIN 25
#define KEY_C3_PIN 33

// Sensores de Presion
#define PRESS_OXIG_PIN 39
#define PRESS_USR_PIN 35
#define PRESS_AMBU_PIN 34
//Valvulas
#define VALV_OXIG_PIN 32 //32  // siempre debe estar
#define VALV_EXTR_PIN 23 // siempre debe estar
//Boton
#define BTN_CFG_PIN 36
//Motor
#define STEPPER1_STEP_PIN 15
#define STEPPER1_DIR_PIN 2
#define STEPPER1_ENA_PIN 4

//#define BUZZ_ALARM_PIN 6
#define HALL_SENS_PIN 16 // siempre debe estar

#endif

#ifdef Arduino_PCB
#define KEY_R1_PIN 13
#define KEY_R2_PIN 12
#define KEY_R3_PIN 11
#define KEY_R4_PIN 10
#define KEY_C1_PIN 9
#define KEY_C2_PIN 8
#define KEY_C3_PIN 7

#define STEPPER1_STEP_PIN 6
#define STEPPER1_DIR_PIN 5
#define STEPPER1_ENA_PIN 4

#define VALV_OXIG_PIN 3   // siempre debe estar
#define VALV_FUELLE_PIN 2 // siempre debe estar

#define PRESS_AMBU_PIN A0
#define PRESS_USR_PIN A1
#define PRESS_OXIG_PIN A2

#define HALL_SENS_PIN 7 // siempre debe estar
#define BUZZ_ALARM_PIN 8
#define BTN_CFG_PIN 9

#endif

#ifdef __Other__
#define KEY_R1_PIN 10
#define KEY_R2_PIN 9
#define KEY_R3_PIN 8
#define KEY_R4_PIN 7
#define KEY_C1_PIN 6
#define KEY_C2_PIN 5
#define KEY_C3_PIN 4

//#define BUZZ_ALARM_PIN 6
#define HALL_SENS_PIN 11 // siempre debe estar
#define VALV_OXIG_PIN 13 // siempre debe estar
#define VALV_EXTR_PIN A3 // siempre debe estar
#define STEPPER1_ENA_PIN A3
#define STEPPER1_STEP_PIN 3
#define STEPPER1_DIR_PIN 2

#define BTN_CFG_PIN 12
#define PRESS_OXIG_PIN A2
#define PRESS_USR_PIN A1
#define PRESS_AMBU_PIN A0

#endif

#endif