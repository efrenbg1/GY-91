////////////////////////////////////////////////////////////////////////////
// This sketch is for the Cansat project of the IES El Burgo de las Rozas //
//    We make it public for helping others that may struggle with the     //
//                             GY-91 sensor : )                           //
// Author: Efren Boyarizo                                                 // 
// Created on: 17/02/2017                                                 //
// Version: 1.2                                                           //
// Notes: -Includes the default config for the GY-91 sensor.              // 
//        -Sends all data via serial                                      //
////////////////////////////////////////////////////////////////////////////


//BMP280

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

int calibrationp = 73;   //Calibration for the pressure
int calibrationa = 97;   //Calibration for the altitude

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10

Adafruit_BMP280 bme;

//MPU9250
#include "mpu.h"
#include "I2Cdev.h"
int ret;




void setup() {
  Serial.begin(9600);
  
  //MPU9250
    Fastwire::setup(400,0);
    ret = mympu_open(200);
    Serial.print("MPU init: "); Serial.println(ret);
    delay(100);  

  //BMP280
  if (!bme.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  
  delay(100);
}

//MPU9250 (some variables, do not erase them!)

unsigned int c = 0; //cumulative number of successful MPU/DMP reads
unsigned int np = 0; //cumulative number of MPU/DMP reads that brought no packet back
unsigned int err_c = 0; //cumulative number of MPU/DMP reads that brought corrupted packet
unsigned int err_o = 0; //cumulative number of MPU/DMP reads that had overflow bit set

void loop() {

    //BMP280
  
    Serial.print(F("T= "));
    Serial.print(bme.readTemperature()); //Do the conversion to F if you need to
    Serial.print(" C ");
    
    Serial.print(F("P= "));
    Serial.print(bme.readPressure()/100+calibrationp); //Should be divided by 100 for showing hPa and not Pa
    Serial.print(" hPa ");
    
    Serial.print(F("A= "));
    Serial.print(bme.readAltitude(1013.25)+calibrationa); // this should be adjusted to your local forcase
    Serial.print(" m ");
    

    //MPU9250

   ret = mympu_update();

   if (!(c%25)) { // output only every 25 MPU/DMP reads
      Serial.print("\tC: "); Serial.print(mympu.ypr[0]); //Compass
      Serial.print(" X: "); Serial.print(mympu.ypr[1]); //X
      Serial.print(" Y: "); Serial.print(mympu.ypr[2]); //Y
      Serial.print("\tgC: "); Serial.print(mympu.gyro[0]); //Acelerometer compass
      Serial.print(" gx: "); Serial.print(mympu.gyro[1]); //Acelerometer X
      Serial.print(" gy: "); Serial.println(mympu.gyro[2]); //Acelerometer Y
      delay(100); 

    }

}
