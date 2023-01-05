/*
 * Smart Composter code
 * Written by: Sung-Joon Im, Carlos Olmos, previous Compostify Team
 */

//==================== Libraries ====================
#include <HTU21D.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//==================== Pins ====================
//OneWire bus pin
#define ONE_WIRE_BUS 2

//==================== Constants ====================
//Humidity percentage low (When is it too dry)
const int dryPercentage = 0.4;

//Humidity percentage high (When is it too wet)
const int wetPercentage = 0.7;

//Temperature low (When is it too cold, Celsius)
const int temperatureLow = 13;

//Temperature high (When is it too hot, Celsius)
const int temperatureHigh = 68;

//==================== Sensors and stuff ====================
//*HTU@1D_F: temperature and humidity sensor*
HTU21D sensor;

//*DS18B20: temperature sensor*
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ==================== Setup and Loop ==================== This whole thing will have to be double checked and revised later on
void setup() 
{
  
  //*HTU@1D_F: temperature and humidity sensor*
  sensor.begin();
  
  Serial.begin(9600); //set the serial port to 9600
  delay(500);
}

void loop() 
{
  //*HTU@1D_F: temperature and humidity sensor*
  float temperature = sensor.getTemperature();
  float humidity = sensor.getHumidity();
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("C");
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  
  //*MQ: gas detect sensor*
  int mq = analogRead(A0);
  Serial.print("Gas sensor value: ");
  Serial.print(mq);
  //over 300 consider as smoke
  if(mq>300)
  {
    Serial.print(" > Smoke detected!");
  }
  Serial.println("");
  
  if(mq<300)
  {
    Serial.print(" > this gas level is fine.");
  }
  Serial.println("");
  
  //*DS18B20: temperature sesnor*
  sensors.requestTemperatures(); 
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempFByIndex(0));
  Serial.println(" F");
  
  //*soil moisure sensor V2.0*
  int soil = analogRead(A0); 
  //translate to scale 0 - 100%
  int percentageHumidity = map(soil, wet, dry, 100, 0);
  Serial.print("Humidity: ");
  Serial.print(percentageHumidity);
  Serial.println("%");
  
  //*HC-SR04: ultrasonic sensor*
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  distance = duration*0.034/2;
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

    
  Serial.println();
  delay(1000);
}
