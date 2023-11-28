#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define SensorPin 36          // the pH meter Analog output is connected with the Arduino’s Analog
#define SENSOR_PIN  12 // ESP32 pin GPIO12 connected to DS18B20 sensor's DATA pin for temperature
int IN4 = 13;         // ESP32 pin GPIO13 connected to DS18B20 sensor's DATA pin for Motor
OneWire oneWire(SENSOR_PIN);
DallasTemperature DS18B20(&oneWire);

float tempC; // temperature in Celsius
float tempF; // temperature in Fahrenheit


unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10], temp;
long previousMillis = 0;
int interval = 5000; // 5 seconds



void setup() {
  Serial.begin(9600); // initialize serial
  DS18B20.begin();    // initialize the DS18B20 sensor
  pinMode(IN4, OUTPUT);
  digitalWrite(IN4, LOW);
}

void loop() {
  phSensorData();
  readTeamperasorSensor();
  delay(1000);
}


void phSensorData()
{
  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buf[i] = analogRead(SensorPin);
    delay(10);
  }
  for (int i = 0; i < 9; i++) //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++)               //take the average value of 6 center sample
    avgValue += buf[i];
  float phValue = (float)avgValue * 5.0 / 1024 / 6; //convert the analog into millivolt
  phValue = 1.5 * phValue - 21.4;                //convert the millivolt into pH value
  Serial.print("    pH:");
  Serial.print(phValue, 2);
  Serial.println(" ");

  if(phValue<5.00){
    motorControl(1);
  }else{
    motorControl(0);
  }
}

void motorControl(int statusMotor) {


  if (statusMotor == 1)                                // if there is data comming
  {
    digitalWrite(IN4, HIGH);
  } else {
    
    digitalWrite(IN4, LOW);
  }
}

void readTeamperasorSensor() {

  DS18B20.requestTemperatures();       // send the command to get temperatures
  tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
  tempF = tempC * 9 / 5 + 32; // convert °C to °F

  Serial.print("Temperature: ");
  Serial.print(tempC);    // print the temperature in °C
  Serial.print("°C");
  Serial.print("  ~  ");  // separator between °C and °F
  Serial.print(tempF);    // print the temperature in °F
  Serial.println("°F");

  

}
