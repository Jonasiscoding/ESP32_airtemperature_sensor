#include <Arduino.h>

//Display
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

//Temperatursensor
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//Bluetooth
#include "BluetoothSerial.h"
String device_name = "Jönes ESP32";
// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif
BluetoothSerial SerialBT;

void setup() {
  u8x8.begin(); //Display
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(1,0,"aktuelle Temp.");

  sensors.begin(); //Temp sensor

  pinMode(25, OUTPUT); //LED green
  pinMode(32, OUTPUT); //LED red
  pinMode(33, OUTPUT); //LED yellow

  SerialBT.begin(device_name); //Bluetooth
}

void loop() {
  sensors.requestTemperatures(); //Send the command to get temperatures
  float tempC = sensors.getTempCByIndex(0); //Temp lesen von Sensor
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.setCursor(5,3);
    u8x8.print(tempC);
  } 
  else
  {
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(3,4,"Error!");
  }
  if(tempC<20)
  {
    digitalWrite(25, HIGH);
    digitalWrite(32, LOW);
    digitalWrite(33, LOW);
  }
  else if((tempC>=20)&&(tempC<=25))
  {
    digitalWrite(33, HIGH);
    digitalWrite(32, LOW);
    digitalWrite(25, LOW);
  }
  else if(tempC>25)
  {
    digitalWrite(32, HIGH);
    digitalWrite(33, LOW);
    digitalWrite(25, LOW);
  }

  SerialBT.print(tempC);
  SerialBT.print("°C\n");

}
