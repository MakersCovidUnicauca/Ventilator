
//#include "Definitions.h"
#include "MenuDefs.h"

void readEncoderUpdate(){
  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    //VERBOSE(newPos);
    if(newPos > pos){
       RotPosition ++;
       down = true;
       //Serial.println("clockwise");
     } else { //Counterclockwise
       up = true;
       //Serial.println("counterclockwise");
       RotPosition--;
     }
     //VERBOSE(RotPosition);
     pos = newPos;
     change = true;
  } // if
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
      RPMVal = RPMMIN; //
    }
  }
  else if (up && page == 2 && menuitem==3 ) {
    up = false;
    IEVal--;
    if(IEVal <= IEMIN){
      IEVal = IEMIN; //
    }
  }
  else if (up && page == 2 && menuitem==4 ) {
    up = false;
    PIPVal--;
    if(PIPVal <= PIPMIN){
      PIPVal = PIPMIN; //
    }
  }
  else if (up && page == 2 && menuitem==5 ) {
    up = false;
    PEEPVal--;
    if(PEEPVal <= PEEPMIN){
      PEEPVal = PEEPMIN; //
    }
  }
  else if (up && page == 2 && menuitem==6 ) {
    up = false;
    POVal--;
    if(POVal <= POMIN){
      POVal = POMIN; //
    }
  }
  else if (up && page == 2 && menuitem==7 ) {
    up = false;
    PRVal--;
    if(PRVal <= PRMIN){
      PRVal = PRMIN; //
    }
  }
  else if (up && page == 2 && menuitem==8 ) {
    up = false;
    VOLVal-=5;
    if(VOLVal <= VOLMIN){
      VOLVal = VOLMIN; //
    }
  }
///////////////////////////////////////////
/////////////////////////////////////////////
  if (down && page == 1) //We have turned the Rotary Encoder Clockwise
  {

    down = false;
    if(menuitem==3 && lastMenuItem == 2)
    {
      frame ++;
      change = true;
    }else  if(menuitem==4 && lastMenuItem == 3)
    {
      frame ++;
      change = true;
    }
    else  if(menuitem==5 && lastMenuItem == 4)
    {
      frame ++;
      change = true;
    }
    else  if(menuitem==6 && lastMenuItem == 5)
    {
      frame ++;
      change = true;
    }
    else  if(menuitem==7 && lastMenuItem == 6 && frame!=6)
    {
      frame ++;
      change = true;
    }
    lastMenuItem = menuitem;
    menuitem++;  
///////////////////////////////////////
///////////////////////////////////////////////////
    
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
    //VERBOSE(RPMVal);
    if (RPMVal >= RPMMAX){
      RPMVal = RPMMAX;
    }
  }
  else if (down && page == 2 && menuitem==3 ) {
    down = false;
    IEVal++;
    //VERBOSE(IEVal);
    if (IEVal >= IEMAX){
      IEVal = IEMAX;
    }
  }
  else if (down && page == 2 && menuitem==4 ) {
    down = false;
    PIPVal++;
    //VERBOSE(PIPVal);
    if (PIPVal >= PIPMAX){
     PIPVal = PIPMAX;
    }
  }
  else if (down && page == 2 && menuitem==5 ) {
    down = false;
    PEEPVal++;
    //VERBOSE(PEEPVal);
    if (PEEPVal >= PEEPMAX){
     PEEPVal = PEEPMAX;
    }
  }
  else if (down && page == 2 && menuitem==6 ) {
    down = false;
    POVal++;
    //VERBOSE(POVal);
    if (POVal >= POMAX){
     POVal = POMAX;
    }
  }
  else if (down && page == 2 && menuitem==7 ) {
    down = false;
    PRVal++;
    //VERBOSE(PRVal);
    if (PRVal >= PRMAX){
     PRVal = PRMAX;
    }
  }
  else if (down && page == 2 && menuitem==8 ) {
    down = false;
    VOLVal+=5;
    if (VOLVal >= VOLMAX){
      VOLVal = VOLMAX;
    }
  }
  
  if (middle) //Middle Button is Pressed
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
    //////
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
    }///////////////
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
  lcd.cursor(); //display.setTextColor(WHITE, BLACK);
  lcd.blink();
}
  
void resetDefaults()
{
// valores por defecto y actualizados por el usuario
  PIPVal = 28; //Peak inspiratory pressure
  PEEPVal = 6; //presion residual en el sistema despues de la fase de exhalacion
  IEVal = 2; //proporcion entre el periodo de inhalacion y el periodo de exhalacion
  POVal = 30;  //porcentaje de oxigeno con respecto al VOL,
  PRVal = 20; //presion del tanque de oxigeno
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
  lcd.cursor(); //display.setTextColor(WHITE, BLACK);
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
  lcd.cursor(); //display.setTextColor(WHITE, BLACK);
  lcd.blink();
  lcd.display();
}

void displayMenuItem(String item, int position, boolean selected)
{
  if(selected)
  {
    posCursor = position; //display.setTextColor(WHITE, BLACK);
  }else
  {
    //lcd.noCursor();//display.setTextColor(BLACK, WHITE);
  }
  lcd.setCursor(0, position);
  lcd.print(">"+item);
}