#include<Arduino.h>
#include "HX711.h" 
#include <SPI.h>
#include <SD.h>

// HX711 circuit wiring
#define LOADCELL1_DOUT_PIN 2
#define LOADCELL1_SCK_PIN  3
#define LOADCELL2_DOUT_PIN 4
#define LOADCELL2_SCK_PIN  5
float reading1;
float reading2;
HX711 LoadCell1,LoadCell2;
float LC1offset1=0.69;
float LC1offset2=0.55;
// float LC2offset1=16100
// float LC2offset2=16100;
float calibrationfactor1=-21500;
// float calibrationfactor2=-21.5541818;

void setup() {
  Serial.begin(115200);
  Serial.print("Initializing SD card...");

  if (!SD.begin(254)) { //BUITLIN_SDCARD-254
    Serial.println("Card failed, or not present");
    while (1);
  }
  File datafile=SD.open("Datafile4.csv",FILE_WRITE);
  Serial.println("card initialized.");
  LoadCell1.begin(LOADCELL1_DOUT_PIN,LOADCELL1_SCK_PIN);
  LoadCell2.begin(LOADCELL2_DOUT_PIN,LOADCELL2_SCK_PIN);
}

void loop() {
    reading1 =LoadCell1.get_value()/calibrationfactor1+LC1offset1+LC1offset2; //In  Kg
    reading2 =LoadCell2.get_value();
    File datafile=SD.open("Datalog4.csv",FILE_WRITE);
    if(datafile){
      unsigned long t=millis();
      datafile.print(t);
      datafile.print(",");
      datafile.print(reading1);
      datafile.print(",");
      datafile.println(reading2);
      Serial.print(t);
      Serial.print(",");
      Serial.print(reading1);
      Serial.print(",");
      Serial.println(reading2);
      datafile.close();
    }
    else{
      Serial.println("ERROR");
      return;
    }
}


