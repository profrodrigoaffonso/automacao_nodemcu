/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

int led = 13;
int bt = 15;
int acionado = 0;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("", "");

  pinMode(led, OUTPUT);
 pinMode(bt, INPUT);
  digitalWrite(led, 1);

}

void loop() {


  if(digitalRead(bt) == HIGH){
    if(acionado == 1){
      acionado = 0;
    } else {
      acionado = 1;
    }
  }

  Serial.println(acionado);
  
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://10.0.0.104/commands/send/1")) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String comando = http.getString();
          if(comando=="on"){
            acionado = 1;
            //digitalWrite(led, 0);
          }
          //Comando para desacionar o módulo relé
          if(comando=="off"){
            acionado = 0;
            //digitalWrite(led, 1);
          }
          Serial.println(comando);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
  Serial.println(acionado);
  if(acionado == 1){
    digitalWrite(led, 0);
  } else {
    digitalWrite(led, 1);
  }
  

  delay(3000);
}
