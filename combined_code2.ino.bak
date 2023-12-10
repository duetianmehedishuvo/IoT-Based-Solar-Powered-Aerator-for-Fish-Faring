#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include "time.h"
#include "DFRobot_ESP_PH.h"
#include "EEPROM.h"

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

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



// Insert your network credentials
#define WIFI_SSID "KIM"
#define WIFI_PASSWORD "KIM654123"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAGDClS7C4rYoVs77kDlcs5gPLPL-_7nNw"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "duetianmehedishuvo@gmail.com"
#define USER_PASSWORD "123456"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "solar-power-aerator-for-79b02-default-rtdb.firebaseio.com"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String tempPath = "/temperature";
String timePath = "/timestamp";

// Parent Node (to be updated in every loop)
String parentPath;

int timestamp;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";
// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 180000;

bool ledStatus = false;


// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}



// TODO:       For dissolved oxygen meter


#define DO_PIN 39

#define VREF 5000    //VREF (mv)
#define ADC_RES 1024 //ADC Resolution

//Single-point calibration Mode=0
//Two-point calibration Mode=1
#define TWO_POINT_CALIBRATION 0

#define READ_TEMP (25) //Current water temperature ℃, Or temperature sensor function

//Single point calibration needs to be filled CAL1_V and CAL1_T
#define CAL1_V (131) //mv
#define CAL1_T (25)   //℃
//Two-point calibration needs to be filled CAL2_V and CAL2_T
//CAL1 High temperature point, CAL2 Low temperature point
#define CAL2_V (4296) //mv
#define CAL2_T (30)   //℃

const uint16_t DO_Table[41] = {
  14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
  11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
  9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
  7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410
};

uint8_t Temperaturet;
uint16_t ADC_Raw;
uint16_t ADC_Voltage;
uint16_t DO;
float dhValue;
float adc_do;

int16_t readDO(uint32_t voltage_mv, uint8_t temperature_c)
{
#if TWO_POINT_CALIBRATION == 00
  uint16_t V_saturation = (uint32_t)CAL1_V + (uint32_t)35 * temperature_c - (uint32_t)CAL1_T * 35;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
#else
  uint16_t V_saturation = (int16_t)((int8_t)temperature_c - CAL2_T) * ((uint16_t)CAL1_V - CAL2_V) / ((uint8_t)CAL1_T - CAL2_T) + CAL2_V;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
#endif
}


void callForDissolvedOxygen()
{
  Temperaturet = (uint8_t)tempC;
  ADC_Raw = analogRead(DO_PIN);
  ADC_Voltage = uint32_t(VREF) * ADC_Raw / ADC_RES;
  adc_do = readDO(ADC_Voltage, Temperaturet);
  float temp1 = Temperaturet;
  float adc_voltage = ADC_Voltage;

  if (ADC_Voltage != 0) {
    dhValue = abs(readDO(ADC_Voltage, Temperaturet) / 100.0);
  }



  Serial.print("Temperaturet:\t" + String(Temperaturet) + "\t");
  Serial.print("ADC RAW:\t" + String(ADC_Raw) + "\t");
  Serial.print("ADC Voltage:\t" + String(ADC_Voltage) + "\t");
  Serial.println("DO:\t" + String(readDO(ADC_Voltage, Temperaturet)) + "\t");

  if (Firebase.RTDB.setFloat(&fbdo, "/SENSOR_DATA/adc_voltage", adc_voltage)) {
    Serial.println("Successfully SAVE TO " + fbdo.dataPath() + " (" + fbdo.dataType() + ")");
  } else {
    Serial.println("FAILED : \n" + fbdo.errorReason());
  }

  if (Firebase.RTDB.setFloat(&fbdo, "/SENSOR_DATA/do", dhValue)) {
    Serial.println("Successfully SAVE TO " + fbdo.dataPath() + " (" + fbdo.dataType() + ")");
  } else {
    Serial.println("FAILED : \n" + fbdo.errorReason());
  }

  if (Firebase.RTDB.setFloat(&fbdo, "/SENSOR_DATA/adc_temperature", temp1)) {
    Serial.println("Successfully SAVE TO " + fbdo.dataPath() + " (" + fbdo.dataType() + ")");
  } else {
    Serial.println("FAILED : \n" + fbdo.errorReason());
  }

  if (dhValue >= 60.0 && dhValue <= 160.0 ) {
    digitalWrite(IN4, LOW);

//    motorControl(0);

  } else {

    

    if (Firebase.RTDB.getString(&fbdo, "/SENSOR_DATA/motor_status")) {
      String analogValue =  fbdo.stringData();
      Serial.println(analogValue);
      if (analogValue == "1") {
        digitalWrite(IN4, HIGH);
      } else {
        digitalWrite(IN4, LOW);
      }

    } else {
      Serial.println("FAILED : \n" + fbdo.errorReason());
    }
  }
}



DFRobot_ESP_PH ph;
#define ESPADC 4096.0   //the esp Analog Digital Convertion value
#define ESPVOLTAGE 3300 //the esp voltage supply value
#define PH_PIN 36    //the esp gpio data pin number
float voltage, phValue, temperature = 27.2;



void setup() {
  Serial.begin(9600); // initialize serial
  DS18B20.begin();    // initialize the DS18B20 sensor
  pinMode(IN4, OUTPUT);
  digitalWrite(IN4, LOW);
  dhValue = 0;
  initWiFi();
  configTime(0, 0, ntpServer);

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }

  EEPROM.begin(32);//needed to permit storage of calibration value in eeprom
  ph.begin();

}

void loop() {
  phSensorData1();
  readTeamperasorSensor();
  callForDissolvedOxygen();
  delay(1000);
}


void phSensorData1()
{
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) //time interval: 1s
  {
    timepoint = millis();
    //voltage = rawPinValue / esp32ADC * esp32Vin
    voltage = analogRead(PH_PIN) / ESPADC * ESPVOLTAGE; // read the voltage
    Serial.print("voltage:");
    Serial.println(voltage, 4);


    phValue = ph.readPH(voltage, tempC); // convert voltage to pH with temperature compensation
    float p = abs(phValue + 2.0);
    Serial.print("pH:");
    Serial.print(p, 4);
    Serial.println("");

    if (Firebase.RTDB.setFloat(&fbdo, "/SENSOR_DATA/ph_value", p)) {
      Serial.println("Successfully SAVE TO " + fbdo.dataPath() + " (" + fbdo.dataType() + ")");
    } else {
      Serial.println("FAILED : \n" + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "/SENSOR_DATA/ph_voltage", (float)voltage)) {
      Serial.println("Successfully SAVE TO " + fbdo.dataPath() + " (" + fbdo.dataType() + ")");
    } else {
      Serial.println("FAILED : \n" + fbdo.errorReason());
    }


  }
  ph.calibration(voltage, temperature); // calibration process by Serail CMD
}

void phSensorData()
{
  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buf[i] = analogRead(36);
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

  if (Firebase.RTDB.setFloat(&fbdo, "/SENSOR_DATA/ph_value", phValue)) {
    Serial.println("Successfully SAVE TO " + fbdo.dataPath() + " (" + fbdo.dataType() + ")");
  } else {
    Serial.println("FAILED : \n" + fbdo.errorReason());
  }

  if (Firebase.RTDB.setFloat(&fbdo, "/SENSOR_DATA/ph_voltage", (float)avgValue)) {
    Serial.println("Successfully SAVE TO " + fbdo.dataPath() + " (" + fbdo.dataType() + ")");
  } else {
    Serial.println("FAILED : \n" + fbdo.errorReason());
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

  if (Firebase.RTDB.setFloat(&fbdo, "/SENSOR_DATA/temperatureC", tempC)) {
    Serial.println("Successfully SAVE TO " + fbdo.dataPath() + " (" + fbdo.dataType() + ")");
  } else {
    Serial.println("FAILED : \n" + fbdo.errorReason());
  }


  if (Firebase.RTDB.setFloat(&fbdo, "/SENSOR_DATA/temperatureF", tempF)) {
    Serial.println("Successfully SAVE TO " + fbdo.dataPath() + " (" + fbdo.dataType() + ")");
  } else {
    Serial.println("FAILED : \n" + fbdo.errorReason());
  }

}
