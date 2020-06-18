//add Button platformio run --target uploadfs
#include <WiFi.h>
#include <WiFiMulti.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

const char *ssid = "STHEVEN-WF";
const char *password = "07crrl11";
const char *ssid1 = "TIGO-5DAC";
const char *password1 = "4NJ567301184";

//WiFiMulti wifiMulti;

const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;


// Globals
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
int led_state = 0;
boolean StateClient = 0;
boolean StatusGraphic = 1;
boolean sendGraphicFlag = 0;
byte numClient = 0;
String msg2Web;
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated
// constants won't change:
const long interval = 1000;

