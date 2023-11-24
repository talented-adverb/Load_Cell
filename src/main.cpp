#include<Arduino.h>
#include "HX711.h" 
#include <SPI.h>
#include <SD.h>

// HX711 circuit wiring
#define LOADCELL1_DOUT_PIN 2
#define LOADCELL1_SCK_PIN  3
// #define LOADCELL2_DOUT_PIN 5
// #define LOADCELL2_SCK_PIN  6
long reading1;
// long reading2;
HX711 LoadCell1,LoadCell2;
long offset1=1.230;
// long offset2=16100;
float calibrationfactor1=-16.0376744;
// float calibrationfactor2=-21.5541818;

void setup() {
  Serial.begin(115200);
  Serial.print("Initializing SD card...");

  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");
  LoadCell1.begin(LOADCELL1_DOUT_PIN,LOADCELL1_SCK_PIN);
  // LoadCell2.begin(LOADCELL2_DOUT_PIN,LOADCELL2_SCK_PIN);
	// LoadCell1.set_offset(offset1);
	// LoadCell2.set_offset(offset2);
}

void loop() {
    String Data1=" ";
    String Data2=" ";
    reading1 =((LoadCell1.get_value()/calibrationfactor1)/1000)+offset1; //In  Kg
    // reading2 =LoadCell2.get_value()/calibrationfactor2;
    Data1+=String(reading1);
    // Data2+=String(reading2);
    File datafile=SD.open("Datalog.txt",FILE_WRITE);
    if(datafile){
      unsigned long t=millis();
      datafile.print(t);
      datafile.print("Loadcell1:");
      datafile.print(Data1);
      datafile.println("Kg");
      // datafile.print("Loadcell2:");
      // datafile.println(Data2);
      Serial.print(t);
      Serial.print(":Loadcell1:");
      Serial.print(Data1);
      Serial.println("Kg");
      // Serial.print(" Loadcell2:");
      // Serial.println(Data2);
      datafile.close();
    }
    else{
      Serial.println("ERROR");
      return;
    }
}


