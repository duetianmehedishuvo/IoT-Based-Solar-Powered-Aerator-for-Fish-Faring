#include <Arduino.h>
 
#define DO_PIN 39
 
#define VREF 5000    //VREF (mv)
#define ADC_RES 1024 //ADC Resolution
 
uint8_t Temperaturet;
uint16_t ADC_Raw;
uint16_t ADC_Voltage;
uint16_t DO;
float DO2;

float dhValue;

 
void setup()
{
  Serial.begin(115200);
  dhValue=0;
}
 
void loop()
{
  Temperaturet = (uint8_t)READ_TEMP;
  ADC_Raw = analogRead(DO_PIN);
  ADC_Voltage = uint32_t(VREF) * ADC_Raw / ADC_RES;

  if(ADC_Voltage!=0){
    dhValue=abs(readDO(ADC_Voltage, Temperaturet)/100.0);
  }

  if(ADC_Voltage<=2000){
    DO2=ADC_Voltage*(1-0.99575172);
  }else if(ADC_Voltage>2000 && ADC_Voltage<=4000){
    DO2=ADC_Voltage*(1-0.997511078);
  }else if(ADC_Voltage>4000 && ADC_Voltage<=6000){
    DO2=ADC_Voltage*(1-0.998152371);
  }else if(ADC_Voltage>6000 && ADC_Voltage<=10000){
    DO2=ADC_Voltage*(1-0.9972315618);
  }else{
    DO2=ADC_Voltage*(1-0.9991858652);
  }
  
  
  
 
//  Serial.print("Temperaturet:\t" + String(Temperaturet) + "\t");
//  Serial.print("ADC RAW:\t" + String(ADC_Raw) + "\t");
  Serial.print("ADC Voltage:\t" + String(ADC_Voltage) + "\t");
  Serial.print("DO 2:\t" + String(DO2) + "\t");
  Serial.print("DO:\t" + String(readDO(ADC_Voltage, Temperaturet)) + "\t");
  Serial.println("DO Voltage:\t" + String(dhValue) + "\t");
 
  delay(1000);
}