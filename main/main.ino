#include "DHTesp.h"

DHTesp dht;

void setup()
{
  Serial.begin(115200);
  dht.setup(14, DHTesp::DHT11); //D5
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  float temperature = dht.getTemperature();
 
  Serial.print("Temperatura w pokoju wynosi :");
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.println();

  delay(1000);
}
