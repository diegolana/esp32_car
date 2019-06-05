#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "index.h"

// Constants
const char *ssid = "ESP32";
const char *password =  "1234567890";
const char *msg_toggle_led = "toggleLED";
const char *msg_get_led = "getLEDState";
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;
const int led_pin = LED_BUILTIN;

// Globals
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
char msg_buf[10];
int led_state = 0;

/***********************************************************
 * Functions
 */

// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {

  // Figure out the type of WebSocket event
  switch(type) {

    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", client_num);
      analogMotorLeft(0);
      analogMotorRight(0);
      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.printf("[%u] Connection from ", client_num);
        Serial.println(ip.toString());
      }
      break;

    // Handle text messages from client
    case WStype_TEXT:
      {
        // Print out raw message
        Serial.printf("[%u] Received text: %s\n", client_num, payload);
  
        char * command = getPayloadName(payload);
        Serial.printf("command: %s\n", command);
       
  
        // Toggle LED
        if ( strcmp(command, "toggleLED") == 0 ) {
          led_state = led_state ? 0 : 1;
          Serial.printf("Toggling LED to %u\n", led_state);
          digitalWrite(led_pin, led_state);
  
        // Report the state of the LED
        } else if ( strcmp(command, "getLEDState") == 0 ) {
          sprintf(msg_buf, "%d", led_state);
          Serial.printf("Sending to [%u]: %s\n", client_num, msg_buf);
          webSocket.sendTXT(client_num, msg_buf);    
  
        // Report Left Motor Value
        } else if ( strcmp(command, "setLeftMotor") == 0 ) {
          int value = getPayloadValue(payload);
          Serial.print("setLeftMotor to ");
          Serial.println(value);
          analogMotorRight(value);
          
        // Report Right Motor Value
        } else if ( strcmp(command, "setRightMotor") == 0 ) {
          int value = getPayloadValue(payload);
          Serial.print("setRightMotor to ");
          Serial.println(value);
          analogMotorLeft(value);
  
        // Message not recognized
        } else {
          Serial.println("Message not recognized\n");
        }
        break;
      }
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

char* getPayloadName(uint8_t * payload) {
    return const_cast<char*>(getValue(String((char *)payload), ':', 0).c_str());
}

int getPayloadValue(uint8_t * payload) {
  String value = getValue(String((char *)payload), ':', 1);
  return value.toInt();
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  //request->send(SPIFFS, "/index.html", "text/html");
  String index = MAIN_page;
  request->send(200, "text/html", index);
  
}

// Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/style.css", "text/css");
}

// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}

/***********************************************************
 * Main
 */

void setup(){

  setupMotors();

  // Init LED and turn off
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  // Start Serial port
  Serial.begin(115200);

  // Make sure we can read the file system
  if( !SPIFFS.begin(true)){
        Serial.println("Error mounting SPIFFS");
        while(1);
  }

  // Start access point
  WiFi.softAP(ssid, password);

  // Print our IP address
  Serial.println();
  Serial.println("AP running");
  Serial.print("My IP address: ");
  Serial.println(WiFi.softAPIP());

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
 
void loop() {
  
  // Look for and handle WebSocket data
  webSocket.loop();
  delay(1);
  //loopMotors();
}
