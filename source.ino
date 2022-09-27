#define THINGER_SERIAL_DEBUG
#include <ThingerESP32.h>
#include "arduino_secrets.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_60X);
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
int moistPin = A6; 
float red, green, blue;  
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

float getTemperature()
{
  sensors.requestTemperatures(); 
  return sensors.getTempCByIndex(0);
}

int getMotor()
{
  return digitalRead(D0);
}

void setup() {

  sensors.begin();  
  pinMode(D0, OUTPUT);
  digitalWrite(D0,LOW);
  Serial.begin(115200);
  if (tcs.begin()) {
    Serial.println("Color sensor found. WOW!");
  } else {
    Serial.println("No TCS34725 color sensor found ... check your connections");
    while (1);
  }
  Serial.println("Moving Forward.");
  thing.add_wifi(SSID, SSID_PASSWORD);
  thing["tomato"] >> [] (pson& out)
  {
    out["temp"] = getTemperature();
    out["moist"] = analogRead(moistPin);
    if (analogRead(moistPin) < 290 )
    {
      digitalWrite(D0, HIGH);
    }
    else {
      digitalWrite(D0, LOW);
    }
    out["motor"] = getMotor();
    tcs.getRGB(&red, &green, &blue);
    out["red"] = int(red);
    out["green"] = int(green);
    out["blue"] = int(blue);
  };
}

void loop() {
  thing.handle();
}
