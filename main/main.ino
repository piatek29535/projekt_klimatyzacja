#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHTesp.h"

#define Fan 0 //D2

ESP8266WebServer server(80);
DHTesp dht;

const char* ssid = "ssid";
const char* password = "password";

void setup(){
  pinMode(Fan,OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.print("\n Adres WiFi: ");
  Serial.print(WiFi.localIP());
  Serial.println();

  server.on("/temperature",temperature);
  server.on("/fanOn",fanOn);
  server.on("/fanOff",fanOff);
  server.onNotFound(notFound);

  server.begin();
  Serial.print("Serwer działa");
  
  dht.setup(14, DHTesp::DHT11); //D5
}

void loop(){
    server.handleClient();
}


//---------- zwraca temperature
void temperature(){
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200,"text/plain", returnTemperature());
}

//---------- włącza wiatrak
void fanOn(){
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200,"text/plain", toggleFan(1));
}

//---------- wyłącza wiatrak
void fanOff(){
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200,"text/plain", toggleFan(0));
}

void notFound(){
  server.send(404, "text/plain", "Błędny adres");
}

String toggleFan(int argument){
  int value;
  
  switch(argument){
    case 1:
      for(value = 0 ; value <= 900; value+=30)
      {
        analogWrite(Fan, value);
        Serial.println(value);
        delay(30);
      }    
      return "Wiatrak wlaczony";
    case 0:
      for(value = 900 ; value >= 0; value-=30)
      {
        analogWrite(Fan, value);
        Serial.println(value);
        delay(30);
      }
      return "Wiatrak wylaczony";
    default:
      return "Default nothing";
  }
}

String returnTemperature(){
  delay(dht.getMinimumSamplingPeriod());

  float temperature = dht.getTemperature();
  
  return (String)temperature;
}
