/*
   Copyright (c) 2019, Vaibhav Kite vaibhavkite3@gmail.com

   Button Relay Control from WIfi AP of esp8266
   Button ON/OFF from web served from esp8266 AP mode

   Generates AP with Name combined with CHIP ID, and
   password. Connecting to it and browsing root page
   gives user to switch ON & OFF the appliances connected
   to respective Relay to GPIO pin combination.

*/

/* Include required libraries */
#include <ESP8266WiFi.h>

/* get chip id of esp */
String AP_CHIP_ID = "ESP_AP_" + String(ESP.getChipId());

/* Create SSID name from CHIP ID */
#ifndef APSSID
#define APSSID AP_CHIP_ID
#define APPSK  "ESP12345"
#endif

/* Setting to created combination of above */
const char *ssid = APSSID.c_str();;
const char *password = APPSK;

/* Initializing web server on port 80 */
WiFiServer server(80);

/* initialiation of varialbles and constants */

//Pin Layout define 4 - Channel 4 - Pin
#define B1 5   //PIN LAYOUT DEFINE D1 to GPIO-5
#define B2 4   //PIN LAYOUT DEFINE D2 to GPIO-4
#define B3 0   //PIN LAYOUT DEFINE D3 to GPIO-0
#define B4 2   //PIN LAYOUT DEFINE D4 to GPIO-2

String B1_STATE = "OFF";
String B2_STATE = "OFF";
String B3_STATE = "OFF";
String B4_STATE = "OFF";

/* ***************SETUP************** */
void setup()
{
  /* serial setup */
  Serial.begin(115200);
  Serial.println("Initiallizing....");
  
  /* setting PIN mode */
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B3, OUTPUT);
  pinMode(B4, OUTPUT);

  /* initializing and assigning pins values */
  digitalWrite(B1, LOW);
  digitalWrite(B2, LOW);
  digitalWrite(B3, LOW);
  digitalWrite(B4, LOW);

  /* Configure SSID */
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);

  /* getting to know the IP address */
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(myIP);

  /* initializing web server */
  server.begin();  // Starts the Server
  Serial.println("Server started");


}

/* ***************LOOP************** */
void loop()
{
  /* server client requests */
  WiFiClient client = server.available();

  /* wait for new client */
  if (!client)
  {
    return;
  }

  /* wait for new client request*/
  Serial.println("Waiting for new client");
  while (!client.available())
  {
    delay(1);
  }

  /* read client non-zero request */
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  /* request responce decision loop */
  // turns the GPIOs on and off
  if (request.indexOf("GET /B1/ON") >= 0) {
    Serial.println("BUTTON B1 on");
    B1_STATE = "ON";
    digitalWrite(B1, HIGH);
  } else if (request.indexOf("GET /B1/OFF") >= 0) {
    Serial.println("BUTTON B1 OFF");
    B1_STATE = "OFF";
    digitalWrite(B1, LOW);
  } else if (request.indexOf("GET /B2/ON") >= 0) {
    Serial.println("BUTTON B2 ON");
    B2_STATE = "ON";
    digitalWrite(B2, HIGH);
  } else if (request.indexOf("GET /B2/OFF") >= 0) {
    Serial.println("BUTTON B2 OFF");
    B2_STATE = "OFF";
    digitalWrite(B2, LOW);
  }else if (request.indexOf("GET /B3/ON") >= 0) {
    Serial.println("BUTTON B3 ON");
    B3_STATE = "ON";
    digitalWrite(B3, HIGH);
  } else if (request.indexOf("GET /B3/OFF") >= 0) {
    Serial.println("BUTTON B3 OFF");
    B3_STATE = "OFF";
    digitalWrite(B3, LOW);
  }else if (request.indexOf("GET /B4/ON") >= 0) {
    Serial.println("BUTTON B4 ON");
    B4_STATE = "ON";
    digitalWrite(B4, HIGH);
  } else if (request.indexOf("GET /B4/OFF") >= 0) {
    Serial.println("BUTTON B4 OFF");
    B4_STATE = "OFF";
    digitalWrite(B4, LOW);
  }

  /*------------------HTML Page Creation---------------------*/

  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  // CSS
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".dot {height: 30px; width: 30px; background-color: #00ff00; border-radius: 100%; display: inline-block;}");
  client.println(".dot_off {background-color: #ff0000;}");
  client.println(".button { background-color: #18c8f1; border: none; color: white; padding: 15px 90px; border-radius: 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button_off {background-color: #404040;}</style></head>");

  // Web Page Heading
  client.println("<body><h1>Control Panel</h1>");

  // Display current state Indicator, and ON/OFF buttons for Button B1
  if (B1_STATE == "OFF") {
    client.println("<h3>BUTTON B1 &nbsp;&nbsp;&nbsp; <span class=\"dot dot_off\"></span></h3>");
  } else {
    client.println("<h3>BUTTON B1 &nbsp;&nbsp;&nbsp; <span class=\"dot\"></span></h3>");
  }  
  // If the B1_STATE is OFF, it displays the ON button
  if (B1_STATE == "OFF") {
    client.println("<p><a href=\"/B1/ON\"><button class=\"button button_off\">OFF</button></a></p>");
  } else {
    client.println("<p><a href=\"/B1/OFF\"><button class=\"button\">ON</button></a></p>");
  }

  // Display current state Indicator, and ON/OFF buttons for Button B2
  if (B2_STATE == "OFF") {
    client.println("<h3>BUTTON B2 &nbsp;&nbsp;&nbsp; <span class=\"dot dot_off\"></span></h3>");
  } else {
    client.println("<h3>BUTTON B2 &nbsp;&nbsp;&nbsp; <span class=\"dot\"></span></h3>");
  }  
  // If the B2_STATE is OFF, it displays the ON button
  if (B2_STATE == "OFF") {
    client.println("<p><a href=\"/B2/ON\"><button class=\"button button_off\">OFF</button></a></p>");
  } else {
    client.println("<p><a href=\"/B2/OFF\"><button class=\"button\">ON</button></a></p>");
  }

  // Display current state Indicator, and ON/OFF buttons for Button B3
  if (B3_STATE == "OFF") {
    client.println("<h3>BUTTON B3 &nbsp;&nbsp;&nbsp; <span class=\"dot dot_off\"></span></h3>");
  } else {
    client.println("<h3>BUTTON B3 &nbsp;&nbsp;&nbsp; <span class=\"dot\"></span></h3>");
  }  
  // If the B3_STATE is OFF, it displays the ON button
  if (B3_STATE == "OFF") {
    client.println("<p><a href=\"/B3/ON\"><button class=\"button button_off\">OFF</button></a></p>");
  } else {
    client.println("<p><a href=\"/B3/OFF\"><button class=\"button\">ON</button></a></p>");
  }

  // Display current state Indicator, and ON/OFF buttons for Button B4
  if (B4_STATE == "OFF") {
    client.println("<h3>BUTTON B4 &nbsp;&nbsp;&nbsp; <span class=\"dot dot_off\"></span></h3>");
  } else {
    client.println("<h3>BUTTON B4 &nbsp;&nbsp;&nbsp; <span class=\"dot\"></span></h3>");
  }  
  // If the B4_STATE is OFF, it displays the ON button
  if (B4_STATE == "OFF") {
    client.println("<p><a href=\"/B4/ON\"><button class=\"button button_off\">OFF</button></a></p>");
  } else {
    client.println("<p><a href=\"/B4/OFF\"><button class=\"button\">ON</button></a></p>");
  }



  client.println("</body></html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
