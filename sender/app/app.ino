#include <WiFi101.h>
#include "WiFiCreds.h"

#define DEBUG

// Battery settings
#define FULL_CHARGE 12.8
#define CHECK_INTERVAL 2000

// Board settings
#define R1 1000000.0
#define R2 220000.0
#define BOARD_VOLTAGE 3.3
#define LED_PIN 12
#define WIFI_CHIPSELECT 8
#define WIFI_IRQ 7
#define WIFI_RESET 4
#define WIFI_ENABLE 2

int wifiStatus = WL_IDLE_STATUS;

void setup() {
  initDebug();
  debug("Starting");
  initLED();
  initWiFi();
}

void loop() {
  float voltage = getVoltage();
  displayVoltage(voltage);
  reportVoltage(voltage);
  delay(CHECK_INTERVAL);
}

void debug(char text[]) {
#ifdef DEBUG
  Serial.println(text);
#endif
}

void debug(float val) {
#ifdef DEBUG
  Serial.println(val);
#endif
}

void initDebug() {
#ifdef DEBUG
  Serial.begin(9600);
  while (!Serial);
#endif
}

void initLED() {
  pinMode(LED_PIN, OUTPUT);
}

void initWiFi() {
  debug("Initializing WiFi");
  WiFi.setPins(WIFI_CHIPSELECT, WIFI_IRQ, WIFI_RESET, WIFI_ENABLE);

  if (WiFi.status() == WL_NO_SHIELD) {
    debug("WiFi shield not present");
    while (true);
  }
  debug("WiFi shield detected");

  while (wifiStatus != WL_CONNECTED) {
    debug("Attempting to connect to WiFi");
    debug(WIFI_SSID);
    wifiStatus = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println(wifiStatus);
    flashLED(2);
    delay(5000);
  }

  debug("Connected to WiFi");
  flashLED(4);
}

void setLED(bool on) {
  digitalWrite(LED_PIN, on ? HIGH : LOW);
}

void flashLED(int count) {
  for (int i = 0; i < count; i++) {
    setLED(true);
    delay(100);
    setLED(false);
    if (i < count - 1) {
      delay(100);
    }
  }
}

float getVoltage() {
  int sensorValue = analogRead(A0);
  return float(sensorValue) * (BOARD_VOLTAGE / 1024.0) / (R2 / (R1 + R2));
}

void displayVoltage(float voltage) {
  if (voltage < FULL_CHARGE * 0.6) {
    flashLED(3);
  } else if (voltage < FULL_CHARGE * 0.8) {
    flashLED(2);
  } else {
    flashLED(1);
  }
  debug(voltage);
}

void reportVoltage(float voltage) {
  debug("TODO: Report voltage");
}
