/*
Smart Composter code
Written by: Sung-Joon Im, Carlos Olmos, previous Compostify Team
*/

//==================== Libraries ====================
#include <OneWire.h>
#include <DallasTemperature.h>

//==================== Constants ====================
//Humidity percentage low (When is it too dry)
const int dryPercentage = 0.4;

//Humidity percentage high (When is it too wet)
const int wetPercentage = 0.7;

//Temperature low (When is it too cold, Celsius)
const int temperatureLow = 13;

//Temperature high (When is it too hot, Celsius)
const int temperatureHigh = 68;

//==================== Pins ====================
//OneWire bus pin (This will be our temperature sensor)
const int ONE_WIRE_BUS 2;

//Moisture sensor pin (analog in)
const int MOISTURE_SENSOR 14;

//Manual tumble push button detection pin (digital in)
const int TUMBLE_BUTTON 13;

//Float switch detection pin (digital in)
const int FLOAT_SWITCH 7;

//Motor transistor (output)
const int MOTOR_TRANSISTOR 6;

//Water pump transistor (output)
const int WATER_PUMP_TRANSISTOR 5;

//LEDs to show the user the state of the system
const int WET_LED 12; //digital output
const int DRY_LED 11; //digital output
const int COLD_LED 10; //digital output
const int HOT_LED 9; //digital output

//==================== Sensors and stuff ====================
//DS18B20: temperature sensor
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ==================== Setup and Loop ==================== This whole thing will have to be double checked and revised later on
void setup() 
{ 
  Serial.begin(9600); //set the serial port to 9600
  delay(500);
}

/*
Read in sensor data
Figure out if the tumbler is too hot, cold, wet, or dry
Activate water pump if too dry
Activate tumbler when hot, cold, wet, or dry
*/
void loop() 
{ 
  //==================== Sensor Readings ====================
  //*DS18B20: temperature sesnor*
  sensors.requestTemperatures(); 
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempFByIndex(0));
  Serial.println(" F");
  
  //*soil moisure sensor V2.0*
  int soil = analogRead(MOISTURE_SENSOR); 
  //translate to scale 0 - 100%
  int percentageHumidity = map(soil, wet, dry, 100, 0);
  Serial.print("Humidity: ");
  Serial.print(percentageHumidity);
  Serial.println("%");

  //==================== Data Processing ====================

  //==================== System and Subsystem activations ====================


  //==================== UI readout ====================
  delay(1000);
}
