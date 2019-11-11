#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHTesp.h"

ESP8266WebServer server(80);
DHTesp dht;

const char* ssid = "ssid";
const char* password = "hasło";

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.print("\n Adres WiFi: ");
  Serial.print(WiFi.localIP());
  Serial.println();

  server.on("/",helloWorld);
  server.onNotFound(notFound);

  server.begin();
  Serial.print("Serwer działa");
  
  dht.setup(14, DHTesp::DHT11); //D5
}

void loop(){
    server.handleClient();
}

void helloWorld(){
  server.send(200,"text/plain", printTemperature());
}

void notFound(){
  server.send(404, "text/plain", "Nie znaju");
}

//use it later 
String printTemperature(){
  delay(dht.getMinimumSamplingPeriod());

  float temperature = dht.getTemperature();

  String message = "Temperatura wynosi: ";
  message += temperature;
  message += " stopni Celsjusza";

  return message;
}
