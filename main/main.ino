#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHTesp.h"

#define Fan 0 //D2

ESP8266WebServer server(80);
DHTesp dht;

const char* ssid = "ssid";
const char* password = "password";
int maxSpeed = 1000;
int minSpeed = 500;
bool isTurnedOn = false;

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
  server.on("/speed",speed);
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

//---------- reguluje prędkość wiatraczka
void speed(){
  server.sendHeader("Access-Control-Allow-Origin", "*");
  
  server.send(200,"text/plain", setSpeed(server.arg(0)));
}

void notFound(){
  server.send(404, "text/plain", "Bledny adres");
}

String toggleFan(int argument){
  int value;
  
  switch(argument){
    case 1:
      for(value = 0 ; value <= maxSpeed; value+=50)
      {
        analogWrite(Fan, value);
        delay(30);
      }    
      isTurnedOn = true;
      return "Wiatrak wlaczony";
    case 0:
      for(value = maxSpeed ; value >= 0; value-=50)
      {
        analogWrite(Fan, value);
        delay(30);
      }
      isTurnedOn = false;
      return "Wiatrak wylaczony";
    default:
      return "Default nothing";
  }
}

String setSpeed(String argument){

  String message = "";
  
  if(isTurnedOn){

    int speed = (argument.toInt()*5)+minSpeed;
    
    if(argument == "0"){
      toggleFan(0);
      message = "Wiatrak wylaczony";
    }else{
      analogWrite(Fan,speed); 
      message = "Predkosc wiatraka ustawiona na: "+(String)speed;
    }

  }else{
    message = "Wlacz wiatrak zeby kontrolowac predkosc";
  }
  
  return message;
}

String returnTemperature(){
  delay(dht.getMinimumSamplingPeriod());

  float temperature = dht.getTemperature();
  
  return (String)temperature;
}
