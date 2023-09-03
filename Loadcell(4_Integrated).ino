/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/

/*
   Settling time (number of samples) and data filtering can be adjusted in the config.h file
   For calibration and storing the calibration value in eeprom, see example file "Calibration.ino"

   The update() function checks for new data and starts the next conversion. In order to acheive maximum effective
   sample rate, update() should be called at least as often as the HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS.
   If you have other time consuming code running (i.e. a graphical LCD), consider calling update() from an interrupt routine,
   see example file "Read_1x_load_cell_interrupt_driven.ino".

   This is an example sketch on how to use this library
*/

#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//pins:
const int HX711_dout1 = 4; //mcu > HX711 dout pin
const int HX711_sck1 = 5; //mcu > HX711 sck pin
const int HX711_dout2 = 6; //mcu > HX711 dout pin
const int HX711_sck2 = 7; //mcu > HX711 sck pinconst int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_dout3 = 8; //mcu > HX711 dout pin
const int HX711_sck3 = 9; //mcu > HX711 sck pin
const int HX711_dout4 = 10; //mcu > HX711 dout pin
const int HX711_sck4 = 11; //mcu > HX711 sck pin
//HX711 constructor:
HX711_ADC LoadCell1(HX711_dout1, HX711_sck1);
HX711_ADC LoadCell2(HX711_dout2, HX711_sck2);
HX711_ADC LoadCell3(HX711_dout3, HX711_sck3);
HX711_ADC LoadCell4(HX711_dout4, HX711_sck4);
// HX711_ADC LoadCell3(HX711_dout3, HX711_sck3);

const int calVal_eepromAdress = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long t3 = 0;
unsigned long t4 = 0;
// unsigned long t3 = 0;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell1.begin();
  LoadCell2.begin();
  LoadCell3.begin();
  LoadCell4.begin();
  // LoadCell3.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue1,calibrationValue2,calibrationValue3,calibrationValue4; // calibration value (see example file "Calibration.ino")
  calibrationValue1 = 21.0; // uncomment this if you want to set the calibration value in the sketch
  calibrationValue2 = 696.0; // uncomment this if you want to set the calibration value in the sketch
  calibrationValue3 = 696.0; // uncomment this if you want to set the calibration value in the sketch
  calibrationValue4 = 696.0; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime1 = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  unsigned long stabilizingtime2 = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  unsigned long stabilizingtime3 = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  unsigned long stabilizingtime4 = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time

  boolean _tare1 = true; //set this to false if you don't want tare to be performed in the next step
  boolean _tare2 = true; //set this to false if you don't want tare to be performed in the next step
  boolean _tare3 = true; //set this to false if you don't want tare to be performed in the next step
  boolean _tare4 = true; //set this to false if you don't want tare to be performed in the next step


  LoadCell1.start(stabilizingtime1, _tare1);
  LoadCell2.start(stabilizingtime2, _tare2);
  LoadCell3.start(stabilizingtime3, _tare3);
  LoadCell4.start(stabilizingtime4, _tare4);


  if (LoadCell1.getTareTimeoutFlag() && LoadCell2.getTareTimeoutFlag() && LoadCell3.getTareTimeoutFlag() && LoadCell4.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell1.setCalFactor(calibrationValue1); // set calibration value (float)
    LoadCell2.setCalFactor(calibrationValue2); // set calibration value (float)
    LoadCell3.setCalFactor(calibrationValue3); // set calibration value (float)
    LoadCell4.setCalFactor(calibrationValue4); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}
void loop() {
  static boolean newDataReady1 = 0;
  static boolean newDataReady2 = 0;
  static boolean newDataReady3 = 0;
  static boolean newDataReady4 = 0;
  // static boolean newDataReady3 = 0;

  const int serialPrintInterval = 0; //increase value to slow down serial print activity
  
  // check for new data/start next conversion:
  if (LoadCell1.update()) newDataReady1 = true;
  if (LoadCell2.update()) newDataReady2 = true;
  if (LoadCell3.update()) newDataReady3 = true;
  if (LoadCell4.update()) newDataReady4 = true;
  // if (LoadCell3.update()) newDataReady3 = true;

  // get smoothed value from the dataset:
  if (newDataReady1 && newDataReady2 && newDataReady3 && newDataReady4) {
    if (millis() > t1 + serialPrintInterval && millis() > t2 + serialPrintInterval && millis() > t3 + serialPrintInterval && millis() > t4 + serialPrintInterval) {
      float i = LoadCell1.getData();
      float j = LoadCell2.getData();
      float k = LoadCell3.getData();
      float l = LoadCell4.getData();

      Serial.print("Load_cell output val1: ");
      Serial.print(i);
      Serial.print("g");
      Serial.print("\tLoad_cell output val2: ");
      Serial.print(j);
      Serial.print("g");
      Serial.print("\tLoad_cell output val3: ");
      Serial.print(k);
      Serial.print("g");
      Serial.print("\tLoad_cell output val4: ");
      Serial.print(l);
      Serial.println("g");
      newDataReady1 = 0;
      newDataReady2 = 0;
      newDataReady3 = 0;
      newDataReady4 = 0;

      t1=t2=t3=t4= millis();
    }
  }

  // receive command from serial terminal, send 't1'|'t2'|'t3' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't1') LoadCell1.tareNoDelay();
  }

  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't2') LoadCell2.tareNoDelay();
  }

  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't3') LoadCell3.tareNoDelay();
  }

  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't4') LoadCell4.tareNoDelay();
  }

  // if (Serial.available() > 0) {
  //   char inByte = Serial.read();
  //   if (inByte == 't3') LoadCell3.tareNoDelay();
  // }

  // check if last tare operation is complete:
  if (LoadCell1.getTareStatus() == true) {
    Serial.println("Tare1 complete");
  }

    if (LoadCell2.getTareStatus() == true) {
    Serial.println("\tTare2 complete");
  }

  if (LoadCell3.getTareStatus() == true) {
    Serial.println("Tare3 complete");
  }

    if (LoadCell4.getTareStatus() == true) {
    Serial.println("\tTare4 complete");
  }

  //   if (LoadCell3.getTareStatus() == true) {
  //   Serial.println("Tare3 complete");
  // }

}
