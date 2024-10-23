//Multi Vital Signs Master Code

#include "DHT.h"
#include <PulseSensorPlayground.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin configs
#define DHTPIN 2    
#define DHTTYPE DHT11   
#define PULSE_PIN 0   
#define LED_PIN 13
#define FADE_PIN 5
#define ONE_WIRE_BUS 4

// Sensors
DHT dht(DHTPIN, DHTTYPE, 2);
PulseSensorPlayground pulseSensor;
int Threshold = 550; 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;
volatile int fadeRate = 0;
volatile boolean QS = false;

void setup() {
  Serial.begin(9600);

  // Initalization
  Serial.println(F("Initializing DHT Sensor..."));
  dht.begin();
  Serial.println(F("Initializing Pulse Sensor..."));
  pulseSensor.analogInput(PULSE_PIN);   
  pulseSensor.blinkOnPulse(LED_PIN);     
  pulseSensor.setThreshold(Threshold);   
  if (pulseSensor.begin()) {
    Serial.println("PulseSensor is initialized!");
  }
  Serial.println(F("Initializing Dallas Temperature Sensor..."));
  sensors.begin();
  if (!sensors.getAddress(insideThermometer, 0)) {
    Serial.println("Unable to find address for Device 0");
  } else {
    sensors.setResolution(insideThermometer, 9);
    Serial.print("Device 0 Resolution: ");
    Serial.println(sensors.getResolution(insideThermometer), DEC);
  }
  pinMode(LED_PIN, OUTPUT);
  pinMode(FADE_PIN, OUTPUT);
}

void loop() {
  //display
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  } else {
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));
  }
  int myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) { 
    Serial.println("♥  A HeartBeat Happened!");
    Serial.print("BPM: ");
    Serial.println(myBPM);
  }
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
  printTemperature(insideThermometer);
  if (QS) {
    fadeRate = 255;
    QS = false;
  }
  ledFadeToBeat();
  delay(2000);
}
void ledFadeToBeat() {
  fadeRate -= 15;
  fadeRate = constrain(fadeRate, 0, 255);
  analogWrite(FADE_PIN, fadeRate);
}
void printTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.println(DallasTemperature::toFahrenheit(tempC));
}
