//Temp HUmidity:
#include <DHT.h>
#include <DHT_U.h>

//For tomato based on paper
int SET_TEMP_L = 65;
int SET_TEMP_H = 85;

int SET_HUM_L = 80;
int SET_HUM_H = 90;

int SET_SOIL_L = 50;


DHT d(2, DHT11);

//Soil:
const int AirValue = 620;   //you need to replace this value with Value_1
const int WaterValue = 310;  //you need to replace this value with Value_2

 //SD:
#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
 // MOSI - pin 11
 // MISO - pin 12
 // CLK - pin 13
 // CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

void setup() {

pinMode(8, OUTPUT);
pinMode(7, OUTPUT);
pinMode(6, OUTPUT);
  
d.begin();
Serial.begin(9600);

//SD card must be in when booting up
  if (!SD.begin(chipSelect)) {
    return;
  }
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "";

  double temp = d.readTemperature(true, true);
  double hum = d.readHumidity();

int soilMoistureValue = 0;
int soilmoisturepercent = 0;

soilMoistureValue = analogRead(A0);  //put Sensor into soil
soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
if(soilmoisturepercent > 100)
{
  soilmoisturepercent = 100;
}
else if(soilmoisturepercent <0)
{
  soilmoisturepercent = 0;
}

if(temp < SET_TEMP_L){
//too cold. heat the plant with electric heater
    digitalWrite(8, HIGH);
} else {
  digitalWrite(8, LOW);
}

if(hum < SET_HUM_L || soilmoisturepercent < SET_SOIL_L || temp > SET_TEMP_H){
//not humid enough. mist
//or
//too hot. cool the plant with mist and fan
  digitalWrite(7, HIGH);
} else {
  digitalWrite(7, LOW);
}

if(hum > SET_HUM_H || temp > SET_TEMP_H){
//too humid. use fan
//or
//too hot. use fan.
  digitalWrite(6, HIGH);
} else {
  digitalWrite(6, LOW);
}

  dataString += "Temp: ";
  dataString += String(temp);

  dataString += ", Humidity: ";
  dataString += String(hum);

  dataString += ", Soil Moisture: ";
  dataString += String(soilmoisturepercent);
  
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("*F    Humidity: ");
  Serial.print(hum);
  Serial.print("%\n");
  Serial.print("Soil Moisture: ");
  Serial.print(soilmoisturepercent);
  Serial.print("%");
  
  Serial.print("\n\n");

  delay(60000);


  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }

}












