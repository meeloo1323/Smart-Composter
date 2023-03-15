/*
Smart Composter code
Written by: Sung-Joon Im, Carlos Olmos, previous Compostify Team
*/

//==================== Libraries ====================
#include <OneWire.h>
#include <DallasTemperature.h>

//==================== Constants ====================
//Moisture percentage low (When is it too dry, in percent)
const int DRY_PERCENTAGE = 40;

//Moisture percentage high (When is it too wet, in percent)
const int WET_PERCENTAGE = 70;

//Moisture reading dry (Sensor raeding when completely dry)
const int DRY_VALUE = 499;

//Moisture reading wet (Sensor reading when in a cup of water)
const int WET_VALUE = 220;

//Temperature low (When is it too cold, Celsius)
const int TEMPERATURE_LOW = 13;

//Temperature high (When is it too hot, Celsius)
const int TEMPERATURE_HIGH = 68;

//How often to poll the sensors (minutes)
const unsigned long POLL_TIME = 1;

//How long to run the tumbling motor (seconds)
const unsigned long TUMBLE_TIME = 60;

//How long to run the water pump (seconds)
const unsigned long WATER_PUMP_TIME = 5;

//==================== Pins ====================
//OneWire bus pin (This will be our temperature sensor)
const int ONE_WIRE_BUS = 2;

//Moisture sensor pin (analog in)
const int MOISTURE_SENSOR = 0;

//Manual tumble push button detection pin (digital in)
const int TUMBLE_BUTTON = 4;

//Float switch detection pin (digital in)
const int FLOAT_SWITCH = 7;

//Motor transistor (output)
const int MOTOR_TRANSISTOR = 6;

//Water pump transistor (output)
const int WATER_PUMP_TRANSISTOR = 5;

//LEDs to show the user the state of the system
const int WET_LED = 12; //digital output
const int DRY_LED = 11; //digital output
const int COLD_LED = 10; //digital output
const int HOT_LED = 9; //digital output
const int EMPTY_TANK_LED = 8; //digital output

//==================== Sensors and stuff ====================
//DS18B20: temperature sensor
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ==================== Setup and Loop ====================
bool tumbleNeeded = false;
bool waterPumpNeeded = false;
bool waterPresenceDetected = false;

//Stores milliseconds for a non-blocking functionality
unsigned long pollMillis = 0; //Sensor polling
unsigned long tumbleMillis = 0; //Tumbler motor on/off
unsigned long waterPumpMillis = 0; //Water pump on/off

void setup() 
{ 
  Serial.begin(9600); //set the serial port to 9600
  delay(500);
  sensors.begin();
  pollMillis = millis();
  pinMode(MOISTURE_SENSOR, INPUT);
  pinMode(TUMBLE_BUTTON, INPUT);
  pinMode(FLOAT_SWITCH, INPUT);

  pinMode(MOTOR_TRANSISTOR, OUTPUT);
  pinMode(WATER_PUMP_TRANSISTOR, OUTPUT);
  pinMode(WET_LED, OUTPUT);
  pinMode(DRY_LED, OUTPUT);
  pinMode(COLD_LED, OUTPUT);
  pinMode(HOT_LED, OUTPUT);
  pinMode(EMPTY_TANK_LED, OUTPUT);
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
  //Polling tumble button regardless of elapsed time to act as an interrupt
  if (digitalRead(TUMBLE_BUTTON))
    tumbleNeeded = true;

  if ((millis() - pollMillis) > (POLL_TIME * 60 * 1000))
  {
    pollMillis = millis();

    //==================== Sensor Readings ====================
    //DS18B20: temperature sensor
    sensors.requestTemperatures();
    int temperature = sensors.getTempCByIndex(0);
    delay(500);

    //Soil moisture sensor
    int temporary = analogRead(MOISTURE_SENSOR);
    delay(500);
    int moisture = map(temporary, WET_VALUE, DRY_VALUE, 100, 0);
    delay(500);

    Serial.println("NEW");
    Serial.println("TEMPERATURE");
    Serial.println(temperature);
    Serial.println("MOISTURE");
    Serial.println(moisture);

    //Water presence detection
    if (digitalRead(FLOAT_SWITCH) == LOW)
      waterPresenceDetected = false;
    else
      waterPresenceDetected = true;

    //==================== Data Processing ====================
    bool wet, dry, cold, hot;
    wet = false;
    dry = false;
    cold = false;
    hot = false;

    //Moisture
    if (WET_PERCENTAGE < moisture)
    {
      wet = true;
      tumbleNeeded = true;
    }

    else if (moisture < DRY_PERCENTAGE)
    {
      dry = true;
      waterPumpNeeded = true;
      tumbleNeeded = true;
    }

    //Temperature
    if (temperature < TEMPERATURE_LOW)
    {
      cold = true;
      tumbleNeeded = true;
    }
    else if (TEMPERATURE_HIGH < temperature)
    {
      hot = true;
      tumbleNeeded = true;
    }
    
    //==================== UI readout ====================
    if (wet)
      digitalWrite(WET_LED, HIGH);
    else
      digitalWrite(WET_LED, LOW);
    
    if (dry)
      digitalWrite(DRY_LED, HIGH);
    else
      digitalWrite(DRY_LED, LOW);
    
    if (cold)
      digitalWrite(COLD_LED, HIGH);
    else
      digitalWrite(COLD_LED, LOW);

    if (hot)
      digitalWrite(HOT_LED, HIGH);
    else
      digitalWrite(HOT_LED, LOW);

    if (!waterPresenceDetected)
      digitalWrite(EMPTY_TANK_LED, HIGH);
    else
      digitalWrite(EMPTY_TANK_LED, LOW);
  }

  //==================== System and Subsystem activations ====================
  if (tumbleNeeded)
    tumbleSystem();

  if (waterPumpNeeded)
    waterPumpSystem();

  //==================== Debugeridoos ====================
  delay(500);
  
}

//==================== Helper Functions ====================

//Tumbler system
//Activates the tumbling system
void tumbleSystem()
{
  //If we're starting the tumble system
  if (tumbleMillis == 0)
  {
    tumbleMillis = millis();
    digitalWrite(MOTOR_TRANSISTOR, HIGH);
  }

  //Otherwise if our tumbling time has been met
  else if (millis() - tumbleMillis >= TUMBLE_TIME * 1000)
  {
    tumbleNeeded = false;
    tumbleMillis = 0;

    digitalWrite(MOTOR_TRANSISTOR, LOW);
  }
}

//Water pump system
//Activates the water pump
void waterPumpSystem()
{
  //If we're starting the water pump system
  if (waterPumpMillis == 0)
  {
    waterPumpMillis = millis();
    digitalWrite(WATER_PUMP_TRANSISTOR, HIGH);
  }

  //Otherwise if our pumping time has been met
  else if (millis() - waterPumpMillis >= WATER_PUMP_TIME * 1000)
  {
    waterPumpNeeded = false;
    waterPumpMillis = 0;

    digitalWrite(WATER_PUMP_TRANSISTOR, LOW);
  }
}
