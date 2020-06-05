# 1 "C:\\Users\\ASUS\\AppData\\Local\\Temp\\tmpdjwdg5la"
#include <Arduino.h>
# 1 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/main.ino"

#include "Definitions.h"
void setup();
void loop();
void readInput();
void readKey();
bool checkVal(byte valType, int valValue);
String checkType(byte valType);
void InitLCDVent();
void refMotorLCD();
void mainMsg();
void updateDisplay(void);
void updateDisplayPressure();
void readEncoderUpdate();
void buttEncChanged(int state);
void updateMenu();
void drawMenu();
void setBlinkCursor(int pos);
void resetDefaults();
void displayIntMenuPage(String menuItem, int value);
void displayStringMenuPage(String menuItem, String value);
void displayMenuItem(String item, int position, boolean selected);
void refMotor();
float GetPosition();
void InitMotor();
void SetMotor(float Distance, float speedM, float accel);
void updateMotorPos();
void buttonChanged(int state);
void storeVarVent();
void readVarVent();
void waitConfig();
void timeoutTI();
void timeoutTH();
void timeoutTE();
void measurePress();
void CtrlPressure();
void MngAssitExh();
void MngAssitInh();
void calculePlateau();
void calculeTime();
void alarma(byte alarm);
void calculeAir();
void calculeOxig();
void stateInit();
void stateConfig();
void stateInhale();
void statePause();
void stateExhale();
void stateListen();
void stateExlPas();
void functInit(void);
void functConfig(void);
void functInhale(void);
void functPause(void);
void functExhale(void);
void functListen(void);
void functExhPas(void);
void functDefault(void);
void updateStateMachine();
void changeState(int newState);
void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t *payload,
                      size_t length);
void onIndexRequest(AsyncWebServerRequest *request);
void onCSSRequest(AsyncWebServerRequest *request);
void onPageNotFound(AsyncWebServerRequest *request);
void setupGraphics();
void loopGraphic();
void StateGraphicSend();
void SendGraphics();
void InitPID();
void CtrlPressurePID();
#line 5 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/main.ino"
void setup()
{
  while (!Serial)
  {
    ;
  }
  Serial.begin(115200);
  Wire.begin();
  delay(1000);
  DEBUG("Ventilador");
  delay(1000);
  DEBUG("testting");
#ifdef Graphic_Serial
  Serial.println("Fuelle,Paciente ");
#endif

  offset = pressAmbu.readCmH2O();
  offset1 = pressUser.readCmH2O();

  pinMode(VALV_OXIG_PIN, OUTPUT);
  pinMode(VALV_EXTR_PIN, OUTPUT);

  pinMode(BTN_CFG_PIN, INPUT);

  pinMode(STEPPER1_STEP_PIN, OUTPUT);
  pinMode(STEPPER1_DIR_PIN, OUTPUT);
  pinMode(STEPPER1_ENA_PIN, OUTPUT);

  pinMode(ENCODER_SW_PIN, INPUT);
  pinMode(ENCODER_DT_PIN, INPUT);
  pinMode(ENCODER_CLK_PIN, INPUT);
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  digitalWrite(27, LOW);
  digitalWrite(26, HIGH);

  pinMode(HALL_SENS_PIN, INPUT_PULLUP);

  digitalWrite(VALV_OXIG_PIN, LOW);
  digitalWrite(VALV_EXTR_PIN, LOW);
  digitalWrite(STEPPER1_ENA_PIN, HIGH);
  InitLCDVent();
  setupGraphics();
  refMotorLCD();
  DEBUG("Serching Motor");
  refMotor();
  DEBUG("Finish Motor");
  currentState = INIT;
  functInit();
  currentInput = Unknown;
}

void loop()
{
# 70 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/main.ino"
  readInput();

  updateStateMachine();

  button.update();

  updateMotorPos();

  Motor.run();

  loopGraphic();
}


void readInput()
{
  asyncTask1.Update(asyncTask2);
  asyncTask2.Update(asyncTask3);
  asyncTask3.Update(asyncTask1);
  asyncTask4.Update();
  asyncTask5.Update();
}
# 1 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/KeypadFunctions.ino"
# 13 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/KeypadFunctions.ino"
#include "Definitions.h"

#ifdef TEST_KEY

void readKey()
{
  String stringKey;
  char customKey = 0;


  if (Serial.available())
  {

    customKey = Serial.read();
  }
  else
  {
    customKey = customKeypad.getKey();
  }

  if (customKey)
  {
    stringKey = "customKey:";
    stringKey = stringKey + String(customKey);
    DEBUG(stringKey);
    switch (customKey)
    {
    case '*':
      stateKey = 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Input Your Type:Value");
      lcd.setCursor(0, 1);
      lcd.print("1.MODE 2.RPM 3.IE");
      lcd.setCursor(0, 2);
      lcd.print("4.PIP 5.PEEP 6.PO");
      lcd.setCursor(0, 3);
      lcd.print("7.PR 8.VOL");
      stringKey = "Input Your Type:Value";
      DEBUG(stringKey);
      break;
    case '#':
      if (stateKey == 1)
      {
        updateDisplay();
      }
      else
      {
        inputCode[pos] = '\0';
        if (pos <= 3)
        {
          int valData = atoi(inputCode);
          stringKey = "value valData:";
          stringKey = stringKey + String(valData);
          DEBUG(stringKey);
          bool check = checkVal(valType, valData);
        }
        else
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("  Input Error!  ");
          lcd.setCursor(0, 1);
          lcd.print("  Please Again  ");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("    Welcome!    ");
        }
      }
      stateKey = 0;
      break;
    default:
      if (stateKey == 1)
      {
        valType = customKey;
        lcd.clear();
        String stringTwo = checkType(customKey);
        stringKey = "valType:";
        stringKey = stringKey + stringTwo;
        DEBUG(stringKey);
        lcd.setCursor(0, 0);
        lcd.print(stringTwo);
        pos = 0;
        stateKey = 2;
      }
      else if (stateKey == 2)
      {
        inputCode[pos] = customKey;
        lcd.setCursor(pos + 6, 0);
        lcd.print(inputCode[pos]);
        pos++;
        if (pos > 3)
        {
          stateKey = 0;
          lcd.setCursor(0, 1);
          lcd.print("Error Value");
        }
      }
    }
  }
}

bool checkVal(byte valType, int valValue)
{

  bool checkVal = false;

  lcd.setCursor(0, 1);

  switch (valType)
  {
  case MDTYPE:
    tempMode = (VentMode)valValue;
    if ((tempMode >= VentMode::CV) && (tempMode <= VentMode::CPA))
    {
      currentVentMode = tempMode;
      lcd.print("MODE:");
      lcd.print(currentVentMode);
      checkVal = true;
    }
    else
    {
      lcd.print("ERROR MODE");
      checkVal = false;
    }
    break;
  case RPMTYPE:
    if ((valValue >= RPMMIN) && (valValue <= RPMMAX))
    {
      RPMVal = valValue;
      lcd.print("RPM:");
      lcd.print(RPMVal);
      checkVal = true;
    }
    else
    {
      lcd.print("ERROR RPM");
      checkVal = false;
    }
    break;
  case IETYPE:
    if ((valValue >= IEMIN) && (valValue <= IEMAX))
    {
      IEVal = valValue;
      lcd.print("IE:");
      lcd.print(IEVal);
      checkVal = true;
    }
    else
    {
      lcd.print("ERROR IE");
      checkVal = false;
    }
    break;

  case PIPTYPE:
    if ((valValue >= PIPMIN) && (valValue <= PIPMAX))
    {
      PIPVal = valValue;
      lcd.print("PIP:");
      lcd.print(PIPVal);
      checkVal = true;
    }
    else
    {
      lcd.print("ERROR PIP");
      checkVal = false;
    }
    break;
  case PEEPTYPE:
    if ((valValue >= PEEPMIN) && (valValue <= PEEPMAX))
    {
      if (PEEPVal < PIPVal)
      {
        PEEPVal = valValue;
        lcd.print("PEEP:");
        lcd.print(PEEPVal);
        checkVal = true;
      }
      else
      {
        lcd.print("PEEP >= PEEP");
        lcd.print(valValue);
        checkVal = false;
      }
    }
    else
    {
      lcd.print("ERROR PEEP");
      checkVal = false;
    }
    break;
  case POTYPE:
    if ((valValue >= POMIN) && (valValue <= POMAX))
    {
      POVal = valValue;
      lcd.print("PO:");
      lcd.print(POVal);
      checkVal = true;
    }
    else
    {
      lcd.print("ERROR PO");
      checkVal = false;
    }
    break;
  case PRTYPE:
    if ((valValue >= PRMIN) && (valValue <= PRMAX))
    {
      PRVal = valValue;
      lcd.print("PR:");
      lcd.print(PRVal);
      checkVal = true;
    }
    else
    {
      lcd.print("ERROR PR");
      checkVal = false;
    }
    break;
  case VOLTYPE:
    if ((valValue >= VOLMIN) && (valValue <= VOLMAX))
    {
      VOLVal = valValue;
      lcd.print("VOL:");
      lcd.print(VOLVal);
      checkVal = true;
    }
    else
    {
      lcd.print("ERROR VOL");
      checkVal = false;
    }
    break;
  default:
    lcd.print("ERROR TYPE");
    checkVal = false;
    break;
  }
  return checkVal;
}

String checkType(byte valType)
{

  String stringOne = "Hello";

  switch (valType)
  {
  case MDTYPE:
    stringOne = String("MODE:");
    break;
  case RPMTYPE:
    stringOne = String("RPM:");
    break;
  case IETYPE:
    stringOne = String("IE:");
    break;
  case PIPTYPE:
    stringOne = String("PIP:");
    break;
  case PEEPTYPE:
    stringOne = String("PEEP:");
    break;
  case POTYPE:
    stringOne = String("PO:");
    break;
  case PRTYPE:
    stringOne = String("PR:");
    break;
  case VOLTYPE:
    stringOne = String("VOL:");
    break;
  default:
    stringOne = String("ERROR TYPE");
  }
  return stringOne;
}

#endif
# 1 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/LCDFunctions.ino"
# 33 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/LCDFunctions.ino"
#include "Definitions.h"

void InitLCDVent(){
  lcd.begin(20, 4);
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("Ventilador Pube ");
  lcd.setCursor(0, 1);
  lcd.print("Init Wifi");
}


void refMotorLCD(){

  lcd.home();
  lcd.clear();
  lcd.print("Ventilador Pube ");
  lcd.setCursor(0, 1);
  lcd.print("ref Motor");
  lcd.setCursor(0, 2);
  lcd.print(WiFi.localIP());
}

void mainMsg(){
  lcd.home();
    lcd.clear();
    lcd.print("Ventilador ");
    lcd.setCursor(0, 1);
    lcd.print("Init ");
    lcd.setCursor(0, 2);
    lcd.print(WiFi.localIP());
}

void updateDisplay(void)
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("MODE:");
  lcd.print(currentVentMode);

  lcd.setCursor(11, 0);
  lcd.print("VOL:");
  lcd.print(VOLVal);

  lcd.setCursor(0, 1);
  lcd.print("RPM:");
  lcd.print(RPMVal);

  lcd.setCursor(7, 1);
  lcd.print("IE:");
  lcd.print(IEVal);
  lcd.setCursor(12, 1);
  lcd.print("PEEP:");
  lcd.print(PEEPVal);

  lcd.setCursor(0, 2);
  lcd.print("PIP:");
  lcd.print(PIPVal);

  lcd.setCursor(7, 2);
  lcd.print("PO:");
  lcd.print(POVal);

  lcd.setCursor(13, 2);
  lcd.print("PR:");
  lcd.print(PRVal);

  lcd.setCursor(0, 3);
  lcd.print("PI:");
  lcd.print(pressInhale);

  lcd.setCursor(6, 3);
  lcd.print("PP:");
  lcd.print(pressPlateau);

  lcd.setCursor(12, 3);
  lcd.print("PE:");
  lcd.print(pressExhale);

#ifdef __DEBG__
  Serial.print("PIP:");
  Serial.print(PIPVal);
  Serial.print(", PEEP:");
  Serial.print(PEEPVal);
  Serial.print(", RPM:");
  Serial.print(RPMVal);
  Serial.print(", VOL:");
  Serial.print(VOLVal);
  Serial.print(", IE:");
  Serial.print(IEVal);
  Serial.print(", PO:");
  Serial.print(POVal);
  Serial.print(", PR:");
  Serial.println(PRVal);
#endif
}

void updateDisplayPressure()
{

  char float_str0[6];
  char line0[21];

  lcd.home();
  lcd.clear();
  lcd.print("Operando");
  lcd.setCursor(0, 1);
  dtostrf(pressInhale, 2, 2, float_str0);
  sprintf(line0, "PInhale: %-5s", float_str0);
  lcd.print(line0);

  lcd.setCursor(0, 2);
  dtostrf(pressPlateau, 2, 2, float_str0);
  sprintf(line0, "PPlateau: %-5s", float_str0);
  lcd.print(line0);

  lcd.setCursor(0, 3);
  dtostrf(pressExhale, 2, 2, float_str0);
  sprintf(line0, "PExhale: %-5s", float_str0);
  lcd.print(line0);

}
# 1 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/Menu.ino"


#include "MenuDefs.h"

void readEncoderUpdate(){
  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {

    if(newPos > pos){
       RotPosition ++;
       down = true;

     } else {
       up = true;

       RotPosition--;
     }

     pos = newPos;
     change = true;
  }
}

void buttEncChanged(int state){

  String stringBtn;
  if(state == HIGH){
    middle=true;
    stringBtn = "Clicked";
  }
  String stringone = "Changed:";
  stringone = stringone + stringBtn;
  DEBUG(stringone);
}

void updateMenu(){

if (up && page == 1 ) {

    up = false;
    if(menuitem==2 && frame ==2)
    {
      frame--;
      change = true;
    }

    if(menuitem==4 && frame ==4)
    {
      frame--;
      change = true;
    }
    if(menuitem==3 && frame ==3)
    {
      frame--;
      change = true;
    }
    if(menuitem==5 && frame ==5)
    {
      frame--;
      change = true;
    }
    if(menuitem==6 && frame ==6)
    {
      frame--;
      change = true;
    }

    lastMenuItem = menuitem;
    menuitem--;
    if (menuitem==0)
    {
      menuitem=1;
    }
  }else if (up && page == 2 && menuitem==1 ) {
    up = false;
    currentVentMode--;
    if(currentVentMode == -1)
    {
      currentVentMode = 3;
    }
  }
  else if (up && page == 2 && menuitem==2 ) {
    up = false;
    RPMVal--;
    if(RPMVal <= RPMMIN){
      RPMVal = RPMMIN;
    }
  }
  else if (up && page == 2 && menuitem==3 ) {
    up = false;
    IEVal--;
    if(IEVal <= IEMIN){
      IEVal = IEMIN;
    }
  }
  else if (up && page == 2 && menuitem==4 ) {
    up = false;
    PIPVal--;
    if(PIPVal <= PIPMIN){
      PIPVal = PIPMIN;
    }
  }
  else if (up && page == 2 && menuitem==5 ) {
    up = false;
    PEEPVal--;
    if(PEEPVal <= PEEPMIN){
      PEEPVal = PEEPMIN;
    }
  }
  else if (up && page == 2 && menuitem==6 ) {
    up = false;
    POVal--;
    if(POVal <= POMIN){
      POVal = POMIN;
    }
  }
  else if (up && page == 2 && menuitem==7 ) {
    up = false;
    PRVal--;
    if(PRVal <= PRMIN){
      PRVal = PRMIN;
    }
  }
  else if (up && page == 2 && menuitem==8 ) {
    up = false;
    VOLVal--;
    if(VOLVal <= VOLMIN){
      VOLVal = VOLMIN;
    }
  }


  if (down && page == 1)
  {

    down = false;
    if(menuitem==3 && lastMenuItem == 2)
    {
      frame ++;
      change = true;
    }else if(menuitem==4 && lastMenuItem == 3)
    {
      frame ++;
      change = true;
    }
    else if(menuitem==5 && lastMenuItem == 4)
    {
      frame ++;
      change = true;
    }
    else if(menuitem==6 && lastMenuItem == 5)
    {
      frame ++;
      change = true;
    }
    else if(menuitem==7 && lastMenuItem == 6 && frame!=6)
    {
      frame ++;
      change = true;
    }
    lastMenuItem = menuitem;
    menuitem++;



    if (menuitem==9)
    {
      menuitem--;
    }

  }else if (down && page == 2 && menuitem==1) {
    down = false;
    currentVentMode++;
    if(currentVentMode == 4)
    {
      currentVentMode = 0;
    }
  }
  else if (down && page == 2 && menuitem==2) {
    down = false;
    RPMVal++;

    if (RPMVal >= RPMMAX){
      RPMVal = RPMMAX;
    }
  }
  else if (down && page == 2 && menuitem==3 ) {
    down = false;
    IEVal++;

    if (IEVal >= IEMAX){
      IEVal = IEMAX;
    }
  }
  else if (down && page == 2 && menuitem==4 ) {
    down = false;
    PIPVal++;

    if (PIPVal >= PIPMAX){
     PIPVal = PIPMAX;
    }
  }
  else if (down && page == 2 && menuitem==5 ) {
    down = false;
    PEEPVal++;

    if (PEEPVal >= PEEPMAX){
     PEEPVal = PEEPMAX;
    }
  }
  else if (down && page == 2 && menuitem==6 ) {
    down = false;
    POVal++;

    if (POVal >= POMAX){
     POVal = POMAX;
    }
  }
  else if (down && page == 2 && menuitem==7 ) {
    down = false;
    PRVal++;

    if (PRVal >= PRMAX){
     PRVal = PRMAX;
    }
  }
  else if (down && page == 2 && menuitem==8 ) {
    down = false;
    VOLVal++;
    if (VOLVal >= VOLMAX){
      VOLVal = VOLMAX;
    }
  }

  if (middle)
  {
    middle = false;

    if (page == 1 && menuitem<=8) {
      page=2;
      change = true;
    }
    else if (page == 2)
    {
      page=1;
      change = true;
    }
  }

}

void drawMenu(){

  if (page==1){
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("MAIN MENU");

    if(menuitem==1 && frame ==1)
    {
      displayMenuItem(menuItem1, 1,true);
      displayMenuItem(menuItem2, 2,false);
      displayMenuItem(menuItem3, 3,false);
    }
    else if(menuitem == 2 && frame == 1)
    {
      displayMenuItem(menuItem1, 1,false);
      displayMenuItem(menuItem2, 2,true);
      displayMenuItem(menuItem3, 3,false);
    }
    else if(menuitem == 3 && frame == 1)
    {
      displayMenuItem(menuItem1, 1,false);
      displayMenuItem(menuItem2, 2,false);
      displayMenuItem(menuItem3, 3,true);
    }
    else if(menuitem == 4 && frame == 2)
    {
      displayMenuItem(menuItem2, 1,false);
      displayMenuItem(menuItem3, 2,false);
      displayMenuItem(menuItem4, 3,true);
    }

    else if(menuitem == 3 && frame == 2)
    {
      displayMenuItem(menuItem2, 1,false);
      displayMenuItem(menuItem3, 2,true);
      displayMenuItem(menuItem4, 3,false);
    }
    else if(menuitem == 2 && frame == 2)
    {
      displayMenuItem(menuItem2, 1,true);
      displayMenuItem(menuItem3, 2,false);
      displayMenuItem(menuItem4, 3,false);
    }

    else if(menuitem == 5 && frame == 3)
    {
      displayMenuItem(menuItem3, 1,false);
      displayMenuItem(menuItem4, 2,false);
      displayMenuItem(menuItem5, 3,true);
    }

    else if(menuitem == 6 && frame == 4)
    {
      displayMenuItem(menuItem4, 1,false);
      displayMenuItem(menuItem5, 2,false);
      displayMenuItem(menuItem6, 3,true);
    }

    else if(menuitem == 5 && frame == 4)
    {
      displayMenuItem(menuItem4, 1,false);
      displayMenuItem(menuItem5, 2,true);
      displayMenuItem(menuItem6, 3,false);
    }
    else if(menuitem == 4 && frame == 4)
    {
      displayMenuItem(menuItem4, 1,true);
      displayMenuItem(menuItem5, 2,false);
      displayMenuItem(menuItem6, 3,false);
    }
    else if(menuitem == 3 && frame == 3)
    {
      displayMenuItem(menuItem3, 1,true);
      displayMenuItem(menuItem4, 2,false);
      displayMenuItem(menuItem5, 3,false);
    }
    else if(menuitem == 4 && frame == 3)
    {
      displayMenuItem(menuItem3, 1,false);
      displayMenuItem(menuItem4, 2,true);
      displayMenuItem(menuItem5, 3,false);
    }

        else if(menuitem == 5 && frame == 5)
    {
      displayMenuItem(menuItem5, 1,true);
      displayMenuItem(menuItem6, 2,false);
      displayMenuItem(menuItem7, 3,false);
    }
        else if(menuitem == 6 && frame == 5)
    {
      displayMenuItem(menuItem5, 1,false);
      displayMenuItem(menuItem6, 2,true);
      displayMenuItem(menuItem7, 3,false);
    }
        else if(menuitem == 7 && frame == 5)
    {
      displayMenuItem(menuItem5, 1,false);
      displayMenuItem(menuItem6, 2,false);
      displayMenuItem(menuItem7, 3,true);
    }
        else if(menuitem == 6 && frame == 6)
    {
      displayMenuItem(menuItem6, 1,true);
      displayMenuItem(menuItem7, 2,false);
      displayMenuItem(menuItem8, 3,false);
    }
        else if(menuitem == 7 && frame == 6)
    {
      displayMenuItem(menuItem6, 1,false);
      displayMenuItem(menuItem7, 2,true);
      displayMenuItem(menuItem8, 3,false);
    }
        else if(menuitem == 8 && frame == 6)
    {
      displayMenuItem(menuItem6, 1,false);
      displayMenuItem(menuItem7, 2,false);
      displayMenuItem(menuItem8, 3,true);
    }

    setBlinkCursor(posCursor);
    lcd.display();
  }
  else if (page==2 && menuitem == 1)
  {
    displayStringMenuPage(menuItem1, ventMode[currentVentMode]);
  }

  else if (page==2 && menuitem == 2)
  {
    displayIntMenuPage(menuItem2, RPMVal);
  }
   else if (page==2 && menuitem == 3)
  {
   displayIntMenuPage(menuItem3, IEVal);
  }
  else if (page==2 && menuitem == 4)
  {
    displayIntMenuPage(menuItem4, PIPVal);
  }
  else if (page==2 && menuitem == 5)
  {
    displayIntMenuPage(menuItem5, PEEPVal);
  }
  else if (page==2 && menuitem == 6)
  {
    displayIntMenuPage(menuItem6, POVal);
  }
  else if (page==2 && menuitem == 7)
  {
    displayIntMenuPage(menuItem7, PRVal);
  }
  else if (page==2 && menuitem == 8)
  {
    displayIntMenuPage(menuItem8, VOLVal);
  }

  }

void setBlinkCursor(int pos){
  lcd.setCursor(0, pos);
  lcd.cursor();
  lcd.blink();
}

void resetDefaults()
{

  PIPVal = 28;
  PEEPVal = 6;
  IEVal = 2;
  POVal = 30;
  PRVal = 20;
  RPMVal = 20;
  VOLVal = 500;
  currentVentMode = 0;
}

void displayIntMenuPage(String menuItem, int value)
{
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print(menuItem);

  lcd.setCursor(1, 1);
  lcd.print("Value");

  lcd.setCursor(1, 2);
  lcd.print(value);

  lcd.setCursor(0, 2);
  lcd.cursor();
  lcd.blink();
  lcd.display();
}

void displayStringMenuPage(String menuItem, String value)
{
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print(menuItem);

  lcd.setCursor(1, 1);
  lcd.print("Value");

  lcd.setCursor(1, 2);
  lcd.print(value);

  lcd.setCursor(0, 2);
  lcd.cursor();
  lcd.blink();
  lcd.display();
}

void displayMenuItem(String item, int position, boolean selected)
{
  if(selected)
  {
    posCursor = position;
  }else
  {

  }
  lcd.setCursor(0, position);
  lcd.print(">"+item);
}
# 1 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/MotorVent.ino"
# 33 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/MotorVent.ino"
#include "Definitions.h"

#define refDistance 70.0
#define refSpeed 20.0
#define refAccel 20.0

void refMotor()
{

  digitalWrite(STEPPER1_ENA_PIN, LOW);

#ifdef InitSensorEnable
  DEBUG("Searching position ");
  SetMotor(refDistance, refSpeed, refAccel);
  while (digitalRead(HALL_SENS_PIN) != HIGH)
  {
    Motor.run();
    delay(10);
  }
  InitMotor();
  DEBUG("Entering back ");
  SetMotor(BACKINITPOS, refSpeed, refAccel);
  while (Motor.isRunning() > 0)
  {
    Motor.run();
    delay(1);
  }
#ifdef __DEBG__
  DEBUG("Got position ");
  Serial.println(GetPosition());
#endif
  SetMotor(refDistance, refSpeed / 10.0, refAccel / 10.0);
  DEBUG("Setting again ");
  while (digitalRead(HALL_SENS_PIN) != HIGH)
  {
    Motor.run();
    delay(1);
  }
  InitMotor();
#else
  digitalWrite(STEPPER1_ENA_PIN, HIGH);
  delay(1000);
  while (digitalRead(HALL_SENS_PIN) != HIGH)
  {
    delay(1000);
    DEBUG("waiting position ");
  }
  InitMotor();
  Serial.println(GetPosition());
#endif
digitalWrite(STEPPER1_ENA_PIN, LOW);

}
# 101 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/MotorVent.ino"
float GetPosition()
{
  float positionMotor = 0;
  positionMotor = Motor.currentPosition() / PulseXmm;
  return positionMotor;
}
# 120 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/MotorVent.ino"
void InitMotor()
{
  Motor.setCurrentPosition(INITPOSITION * PulseXmm);
}
# 140 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/MotorVent.ino"
void SetMotor(float Distance, float speedM, float accel)
{
  long DistanceValue = (long)Distance * PulseXmm;
  Motor.setMaxSpeed(speedM * PulseXmm);
  Motor.setAcceleration(accel * PulseXmm);
  Motor.moveTo(DistanceValue);
}
# 160 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/MotorVent.ino"
void updateMotorPos()
{
  int dist = (int)GetPosition();
  if (mPosCurrent != dist)
  {
    String stringone = "Dist:";
    stringone = stringone + String(dist);
    mPosCurrent = dist;

  }
}
# 1 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/Other.ino"

#include "Definitions.h"

void buttonChanged(int state){
  String stringBtn;
  if((state == HIGH) && (currentState == INIT) ){
    currentInput = SMInput::BtnReset;
    stringBtn = "input_reset";
  }
  else if((state == LOW) && (currentState == CONFIG) ){
    currentInput = SMInput::BtnConfig;
    stringBtn = "BtnConfig";
    mainMsg();
  }
  else if((state == LOW) && (currentState == INHALE)){
    currentInput = SMInput::BtnReset;
    asyncTask1.Stop();
    asyncTask2.Stop();
    asyncTask3.Stop();
    Motor.stop();
    stringBtn = "input_reset";
  }
  else if((state == LOW) && (currentState == EXHALE)){
    currentInput = SMInput::BtnReset;
    asyncTask1.Stop();
    asyncTask2.Stop();
    asyncTask3.Stop();
    Motor.stop();
    stringBtn = "input_reset";
  }

  String stringone = "Changed:";
  stringone = stringone + String(state) + stringBtn;
  DEBUG(stringone);
}

void storeVarVent(){

  EEPROM.write(addrVarVent, MRK_STR_VAR);
  EEPROM.write(MDTYPE, currentVentMode);
  EEPROM.write(RPMTYPE, RPMVal);
  EEPROM.write(IETYPE, IEVal);

  EEPROM.write(PIPTYPE, PIPVal);
  EEPROM.write(PEEPTYPE, PEEPVal);
  EEPROM.write(POTYPE, POVal);
  EEPROM.write(PRTYPE, PRVal);
  EEPROM.put(VOLTYPE, VOLVal);
}



void readVarVent(){

  byte value = EEPROM.read(addrVarVent);
  if(value == MRK_STR_VAR){
    currentVentMode = (VentMode)EEPROM.read(MDTYPE);
    RPMVal = EEPROM.read(RPMTYPE);
    IEVal = EEPROM.read(IETYPE);

    PIPVal = EEPROM.read(PIPTYPE);
    PEEPVal = EEPROM.read(PEEPTYPE);
    POVal = EEPROM.read(POTYPE);
    PRVal = EEPROM.read(PRTYPE);
    EEPROM.get(VOLTYPE, VOLVal);
  }
}
# 1 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/StateMachine.ino"



#include "Definitions.h"


void waitConfig()
{
  DEBUG("TI_CONFIG");
  currentInput = SMInput::BtnConfig;
}

void timeoutTI()
{
  DEBUG("TI_END");
  currentInput = SMInput::TIEnd;
}

void timeoutTH()
{
  DEBUG("TH_END");
  currentInput = SMInput::THEnd;
}

void timeoutTE()
{
  DEBUG("TE_END");
  currentInput = SMInput::TEEnd;
}

void measurePress()
{

  pAmbu = pressAmbu.readCmH2O() - offset;
  pressureUser = pressUser.readCmH2O() - offset1;

  if (FlagAire == true)
  {
    if(pAmbu == 0.0)
    {
      DEBUG("END OXY+AIR");

      FlagAire = false;


    }
  }


  if (FlagOxig == true)
  {
    if(pAmbu == 0.0){
      calculeAir();


    }
    float pOxig = pressOxig.readCmH2O();
    if (pOxig < PRVal)
    {

    }
  }

  FlagPressure = true;

#ifdef Graphic_Serial
    Serial.print(pInh, 3);
    Serial.print(",");
    Serial.print(GetPosition());
    Serial.println(" ");
#endif
  preUser[TimestoSend] = pAmbu;
  volUser[TimestoSend] = GetPosition();
  TimestoSend++;
  if (TimestoSend >= TimeSendGraphic)
  {
    TimestoSend = 0;
#ifdef __DEBG__







#endif
  sendGraphicFlag = 1;
  }

}

void CtrlPressure()
{
  if (currentVentMode == VentMode::CP)
  {
    if (pressureUser >= PIPVal)
    {
      DEBUG("PIP_DETEC");
      Motor.stop();
      asyncTask2.Stop();
      asyncTask3.Start();
      currentInput = SMInput::TIEnd;
    }
  }
}

void MngAssitExh()
{
  if (FlagPressure)
  {
    if (pressureUser < pressExhale)
    {
      pressExhale = pressureUser;
    }
    if ((currentVentMode == VentMode::CVA) || (currentVentMode == VentMode::CPA))
    {
      if (pressExhale >= PLI_FAB)
      {
        DEBUG("PLI_DETEC");
        asyncTask1.Stop();
        asyncTask2.Start();
        currentInput = SMInput::TEEnd;
      }
    }
    FlagPressure = false;
  }
}

void MngAssitInh()
{
  if (FlagPressure)
  {
    if (pressureUser > pressInhale)
    {
      pressInhale = pressureUser;
    }
    if ((currentVentMode == VentMode::CVA) || (currentVentMode == VentMode::CPA))
    {
      if (pressInhale >= PLI_FAB)
      {
        DEBUG("TI_END");
        asyncTask2.Stop();
        asyncTask3.Start();
        currentInput = SMInput::TIEnd;
      }
    }
    FlagPressure = false;
  }
}

void calculePlateau()
{
  if (FlagPressure)
  {

    if (pressureUser != pressPlateau)
    {
      pressPlateau = pressureUser;
    }
    else
    {
      idxPlateau++;
    }
    if (idxPlateau == 4)
    {
      String stringone = "Plateau:";
      stringone = stringone + String(pressPlateau);
      DEBUG(stringone);
    }
    FlagPressure = false;
  }
}

void calculeTime()
{
  DEBUG("CTIME");
  TVal = (TIMESEC / RPMVal);
  TIVal = TVal / (1 + IEVal);
  TEVal = TVal - TIVal;
  TI = TIVal - THVal;

  asyncTask1.SetIntervalMillis(TEVal);
  asyncTask2.SetIntervalMillis(TI);
  asyncTask3.SetIntervalMillis(THVal);

#ifdef __DEBG__
  Serial.print("T: ");
  Serial.print(TVal);
  Serial.print(" TI: ");
  Serial.print(TI);
  Serial.print(" TP: ");
  Serial.print(THVal);
  Serial.print(" TE: ");
  Serial.println(TEVal);
#endif
}

void alarma(byte alarm)
{
  String stringAlarm = "ALARMA:";
  String stringone;
  switch (alarm)
  {
  case PWR:
    stringone = "POWER";
    break;
  case OXY:
    stringone = "OXYGENO";
    break;
  case ALM:
    stringone = "ALARMA:";
    break;
  case TEST:
    stringone = "TEST:";
    break;
  }
  stringAlarm = stringAlarm + stringone;
  DEBUG(stringAlarm);
}

void calculeAir()
{
  if ((Motor.isRunning() == 0) && (FlagAire == false))
  {
    DEBUG("AIRE");
    float Vol;
    float Po = POVal * 0.01;
    if ((currentVentMode == VentMode::CV) || (currentVentMode == VentMode::CVA))
    {
      Vol = VOLVal;
    }
    else if ((currentVentMode == VentMode::CP) || (currentVentMode == VentMode::CPA))
    {
      Vol = VOLMAX;
    }
    else{
      Vol = VOLVal;
    }



    mPosEnd = float(Vol / RELMMVOL);
    DistMotor = INITPOSITION - mPosEnd;
    VelMotor = float(mPosEnd / ((TIVal * (1 - Po)) / 1000.0));
    AcelMotor = VelMotor * 20;
    SetMotor(DistMotor, VelMotor, AcelMotor);


    digitalWrite(VALV_OXIG_PIN, LOW);
    FlagOxig = false;
    FlagAire = true;

#ifdef __DEBG__
    Serial.print(" DistMotor: ");
    Serial.print(DistMotor);
    Serial.print(" mPosEnd: ");
    Serial.print(mPosEnd);
    Serial.print(" Po: ");
    Serial.print(Po);
    Serial.print(" mPosOxi: ");
    Serial.print(mPosOxi);
    Serial.print(" DISTM: ");
    Serial.print(DistMotor);
    Serial.print(" VelM: ");
    Serial.print(VelMotor, 3);
    Serial.print(" AccelM: ");
    Serial.println(AcelMotor, 3);
#endif
  }
}

void calculeOxig()
{

  DEBUG("OXYGENO");
  float Vol;
  float Po = POVal * 0.01;

  if ((currentVentMode == VentMode::CV) || (currentVentMode == VentMode::CVA))
  {
    VOLRes = (mPosCurrent - INITPOSITION) * RELMMVOL;
    VOLRes = 0;
    Vol = VOLVal - VOLRes;
  }
  else if ((currentVentMode == VentMode::CP) || (currentVentMode == VentMode::CPA))
  {
    Vol = VOLMAX;
  }
  else{
    Vol = VOLVal;
  }



  mPosOxi = Vol - float(((1 - Po) * Vol / 0.79));
  mPosOxi = (mPosOxi / RELMMVOL);
  DistMotor = INITPOSITION - mPosOxi;

  VelMotor = mPosOxi / (TIVal / 1000.0);
  AcelMotor = VelMotor * 20;
  SetMotor(DistMotor, VelMotor, AcelMotor);


  digitalWrite(VALV_OXIG_PIN, HIGH);
  FlagOxig = true;
# 317 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/StateMachine.ino"
#ifdef __DEBG__
  Serial.print("VOLRes: ");
  Serial.print(VOLRes);
  Serial.print(" Vol: ");
  Serial.print(Vol);
  Serial.print(" Po: ");
  Serial.print(Po);
  Serial.print(" mPosOxi: ");
  Serial.print(mPosOxi);

  Serial.print(" DISTM: ");
  Serial.print(DistMotor);
  Serial.print(" VelM: ");
  Serial.print(VelMotor, 3);
  Serial.print(" AccelM: ");
  Serial.println(AcelMotor, 3);
#endif
}

void stateInit()
{
  if (currentInput == SMInput::BtnReset)
    changeState(SMState::CONFIG);
  if (currentInput == SMInput::BtnConfig)
    changeState(SMState::EXHALE);
}


void stateConfig()
{
  if (currentInput == SMInput::BtnConfig)
    changeState(SMState::EXHALE);

  #ifdef TEST_KEY
  readKey();
  #endif
  #ifdef TEST_ENC
  buttEnc.update();
  readEncoderUpdate();
  updateMenu();
  if (change == true){
    drawMenu();
    change = false;
  }
  #endif
}

void stateInhale()
{
  if (currentInput == SMInput::TIEnd)
    changeState(SMState::PAUSE);
  if (currentInput == SMInput::BtnReset)
    changeState(SMState::CONFIG);

#ifdef TEST_MODE
  CtrlPressure();
  MngAssitInh();
#endif
}

void statePause()
{
  if (currentInput == SMInput::THEnd)
    changeState(SMState::EXHALE);
  if (currentInput == SMInput::BtnReset)
    changeState(SMState::CONFIG);
#ifdef TEST_SENSOR
  calculePlateau();
#endif
}

void stateExhale()
{
  if (currentInput == SMInput::TEEnd)
    changeState(SMState::INHALE);
  if (currentInput == SMInput::BtnReset)
    changeState(SMState::CONFIG);
#ifdef TEST_MOTOR
  calculeAir();
#endif
#ifdef TEST_MODE
  MngAssitExh();
#endif
}

void stateListen()
{
}

void stateExlPas()
{
}

void functInit(void)
{
  DEBUG("INIT");
  mainMsg();
  button.setCallback(buttonChanged);

  buttEnc.setCallback(buttEncChanged);

  asyncTask5.Start();

}

void functConfig(void)
{
  DEBUG("CONFIG");
  readVarVent();
  drawMenu();
  change = false;
}

void functInhale(void)
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

void functPause(void)
{
  DEBUG("PAUSE");
#ifdef TEST_SENSOR
  pressPlateau = pressureUser;
  idxPlateau = 0;
#endif
}

void functExhale(void)
{
  DEBUG("EXHALE");
  if (currentInput == SMInput::BtnConfig)
  {
#ifdef TEST_MODE
    if ((PMVal == 0) && (currentVentMode >= VentMode::CP))
    {
      currentVentMode = VentMode::CV;
    }
#endif
    calculeTime();
#ifdef TEST_SENSOR
    asyncTask4.Start();
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
  calculeOxig();

#endif

  asyncTask1.Start();
}

void functListen(void)
{
  DEBUG("LISTEN");
}

void functExhPas(void)
{
  DEBUG("EXHALE PAUSE");
}

void functDefault(void)
{
  DEBUG("ERROR STATE");
}


void updateStateMachine()
{
  switch (currentState)
  {
  case INIT:
    stateInit();
    break;
  case CONFIG:
    stateConfig();
    break;
  case INHALE:
    stateInhale();
    break;
  case PAUSE:
    statePause();
    break;
  case EXHALE:
    stateExhale();
    break;
  case LISTEN:
    stateListen();
    break;
  case EXLPAS:
    stateExlPas();
    break;
  }
}


void changeState(int newState)
{
  currentState = (SMState)newState;

  switch (currentState)
  {
  case SMState::INIT:
    functInit();
    break;
  case SMState::CONFIG:
    functConfig();
    break;
  case SMState::INHALE:
    functInhale();
    break;
  case SMState::PAUSE:
    functPause();
    break;
  case SMState::EXHALE:
    functExhale();
    break;
  case SMState::LISTEN:
    functListen();
    break;
  case SMState::EXLPAS:
    functExhPas();
    break;
  default:
    functDefault();
    break;
  }
}
# 1 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/WebsocketVent.ino"



#include "Definitions.h"
# 13 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/WebsocketVent.ino"
void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t *payload,
                      size_t length)
{


  numClient = client_num;
  switch (type)
  {


  case WStype_DISCONNECTED:

    StateClient = 0;
    break;


  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(client_num);

    DEBUG(ip.toString());
    StateClient = 1;
  }
  break;

  case WStype_TEXT:


    if (strcmp((char *)payload, "EndGraphic") == 0)
    {
      StateClient = 1;

    }
    else{
      if(strcmp((char *)payload, "getStatus") == 0)
      {
        StatusGraphic = 1;
      }
      else{
        DEBUG(" Message not recognized");
      }
    }
    break;

  case WStype_BIN:
  case WStype_ERROR:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
  default:
    break;
  }
}


void onIndexRequest(AsyncWebServerRequest *request)
{
  IPAddress remote_ip = request->client()->remoteIP();
  DEBUG("[" + remote_ip.toString() +
                 "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}


void onCSSRequest(AsyncWebServerRequest *request)
{
  IPAddress remote_ip = request->client()->remoteIP();
  DEBUG("[" + remote_ip.toString() +
                 "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/style.css", "text/css");
}


void onPageNotFound(AsyncWebServerRequest *request)
{
  IPAddress remote_ip = request->client()->remoteIP();
  DEBUG("[" + remote_ip.toString() +
                 "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}






void setupGraphics()
{

  msg2Web.reserve(700);

  if (!SPIFFS.begin())
  {
    DEBUG("Error mounting SPIFFS");
    while (1)
      ;
  }

  WiFi.begin(ssid1, password1);

  byte counterWifi = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    DEBUG(".");
    counterWifi++;
    if(counterWifi > 200){
      ESP.restart();
    }
  }
  DEBUG("Connected to ");
  DEBUG(ssid1);
  DEBUG("IP address: ");
  DEBUG(WiFi.localIP());


  server.on("/", HTTP_GET, onIndexRequest);

  server.on("/style.css", HTTP_GET, onCSSRequest);

  server.onNotFound(onPageNotFound);

  server.begin();

  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

char buff[15];

void loopGraphic(){
  webSocket.loop();
  if(sendGraphicFlag){
    sendGraphicFlag = 0;
    if(StateClient){
      if(StatusGraphic){
        StatusGraphic=0;
        StateGraphicSend();
      }
      else{
          SendGraphics();
      }
      webSocket.sendTXT(numClient, msg2Web);
      StateClient = 0;
    }
  }
}
void StateGraphicSend(){

      msg2Web = "c, Volumen Controlado";
      msg2Web += "," + String(PIPVal);
      msg2Web += "," + String(PEEPVal);
      msg2Web += "," + String(RPMVal);
      msg2Web += "," + String(VOLVal);
      msg2Web += "," + String(IEVal);
      dtostrf( 0.025, 1, 3, buff);
      msg2Web += "," + String(buff);
}


byte multi = 1;
void SendGraphics()
{
    msg2Web = "v";
    for (int i = 0; i < TimeSendGraphic; i ++)
      {
        msg2Web += "," + String(volUser[i]);
      }
    DEBUG(msg2Web);
    webSocket.sendTXT(numClient, msg2Web);
    msg2Web = "p";
    for (int i = 0; i < TimeSendGraphic; i++)
      {
        msg2Web += "," + String(preUser[i]);
      }
      multi += 5;
    DEBUG(msg2Web);


}
# 1 "C:/Users/ASUS/Documents/GitHub/Ventilator/src/motorPID.ino"


#include "Definitions.h"
#include <PID_v1.h>


double Setpoint, Input, Output;


double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;


PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

void InitPID(){

  Setpoint = PIPVal;


  myPID.SetMode(AUTOMATIC);

}

void CtrlPressurePID()
{



  DEBUG("PM_DETEC");

  if (currentVentMode == VentMode::CP)
  {
    Input = pAmbu;

    double gap = abs(Setpoint-Input);
    if (gap < 10)
    {
        myPID.SetTunings(consKp, consKi, consKd);
    }
    else
    {

        myPID.SetTunings(aggKp, aggKi, aggKd);
    }

    myPID.Compute();

    VelMotor = Output;
    Motor.setMaxSpeed(VelMotor * PulseXmm);

  }
}