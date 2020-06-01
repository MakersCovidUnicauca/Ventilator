

//*****************************************************************************/
#include "Definitions.h"



/***********************************************************
   Functions
*/

// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t *payload,
                      size_t length)
{

  // Figure out the type of WebSocket event
  numClient = client_num;
  switch (type)
  {

  // Client has disconnected
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", client_num);
    StateClient = 0;
    break;

  // New client has connected
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(client_num);
    Serial.printf("[%u] Connection from ", client_num);
    Serial.println(ip.toString());
    StateClient = 1;
  }
  break;
  // Handle text messages from client
  case WStype_TEXT:
    // Print out raw message
    Serial.printf("[%u] Received text: %s\n", client_num, payload);
    if (strcmp((char *)payload, "EndGraphic") == 0)
    {
      StateClient = 1;
      // Ready to send the next message
    }
    else{
      if(strcmp((char *)payload, "getStatus") == 0)
      {
        StatusGraphic = 1;
      }
      else{
        Serial.println(" Message not recognized");
      }
    }
    break;
  // For everything else: do nothing
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

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request)
{
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                 "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}

// Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest *request)
{
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                 "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/style.css", "text/css");
}

// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request)
{
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                 "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}

/***********************************************************
   Main
*/


void setupGraphics()
{
  // Start Serial port
  msg2Web.reserve(700);
  // Make sure we can read the file system
  if (!SPIFFS.begin())
  {
    Serial.println("Error mounting SPIFFS");
    while (1)
      ;
  }

  WiFi.begin(ssid, password);
  // Wait for connection
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
  // Print our IP address
  // On HTTP request for root, provide index.html file
  server.on("/", HTTP_GET, onIndexRequest);
  // On HTTP request for style sheet, provide style.css
  server.on("/style.css", HTTP_GET, onCSSRequest);
  // Handle requests for pages that do not exist
  server.onNotFound(onPageNotFound);
  // Start web server
  server.begin();
  // Start WebSocket server and assign callback
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
      msg2Web += "," + String(PIPVal);   //PIP
      msg2Web += "," + String(PEEPVal);   //PEEP
      msg2Web += "," + String(RPMVal);    //RPM
      msg2Web += "," + String(VOLVal);   //Volumen
      msg2Web += "," + String(IEVal);     //IE
      dtostrf( 0.025, 1, 3, buff);
      msg2Web += "," + String(buff);
}


byte multi = 1;
byte var2Send = 0;
void SendGraphics()
{
  int value = 400;
  float senoFun = 0.0;
  if(var2Send == 0){
      msg2Web = "v";
    for (int i = 0; i < 40; i ++)
      {
        senoFun = sin((i + multi) * 0.0174533);
        msg2Web += "," + String(volUser[i]);
      }
      Serial.println(msg2Web);
      var2Send =1;
  }
  else{
     msg2Web = "p";
    for (int i = 0; i < value; i += 10)
      {
        senoFun = cos((i + multi) * 0.0174533);
        msg2Web += "," + String(i*0.02);
      }
      multi += 5;
      Serial.println(msg2Web);
      var2Send =0;
  }

    
   
    /*
    for (int i = 0; i < TimeSendGraphic; i ++)
    {
      dtostrf( volUser[i], 1, 1, buff);
      msg2Web += "," + String(buff);
    }
    webSocket.sendTXT(numClient, msg2Web);
    msg2Web = "p";
    for (int i = 0; i < TimeSendGraphic; i ++)
    {
      dtostrf( preUser[i], 1, 1, buff);
      msg2Web += "," + String(buff);
    }
    */

  //Serial.println(msg2Web);
 
}

