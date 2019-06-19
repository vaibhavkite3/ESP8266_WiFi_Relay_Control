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

//Pin Layout define 2 - Channel 2 - Pin
#define B1 5   //PIN LAYOUT DEFINE D1 to GPIO-5
#define B2 4   //PIN LAYOUT DEFINE D2 to GPIO-4

String B1_STATE = "OFF";
String B2_STATE = "OFF";

/* ***************SETUP************** */
void setup()
{
  /* serial setup */
  Serial.begin(115200);
  Serial.println("Initiallizing....");
  
  /* setting PIN mode */
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);

  /* initializing and assigning pins values */
  digitalWrite(B1, LOW);
  digitalWrite(B2, LOW);

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
  }

  /*------------------HTML Page Creation---------------------*/

  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  // CSS
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button_off {background-color: #77878A;}</style></head>");

  // Web Page Heading
  client.println("<body><h1>Control Panel</h1>");

  // Display current state, and ON/OFF buttons for Button B1
  client.println("<p>BUTTON B1 - State " + B1_STATE + "</p>");
  // If the B1_STATE is OFF, it displays the ON button
  if (B1_STATE == "OFF") {
    client.println("<p><a href=\"/B1/ON\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/B1/OFF\"><button class=\"button button_off\">OFF</button></a></p>");
  }

  // Display current state, and ON/OFF buttons for Button B2
  client.println("<p>BUTTON B2 - State " + B2_STATE + "</p>");
  // If the B2_STATE is OFF, it displays the ON button
  if (B2_STATE == "OFF") {
    client.println("<p><a href=\"/B2/ON\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/B2/OFF\"><button class=\"button button_off\">OFF</button></a></p>");
  }
  client.println("</body></html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
