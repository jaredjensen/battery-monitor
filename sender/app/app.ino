#include <b64.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <SPI.h>
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
#define WIFI_CHIPSELECT 8
#define WIFI_IRQ 7
#define WIFI_RESET 4
#define WIFI_ENABLE 2

// LED RGB pins
#define RGB 0b1010000000010000000
#define RED 0b0000000000010000000
#define GRN 0b1000000000000000000
#define BLU 0b0010000000000000000
#define OFF 0b0000000000000000000

byte mac[] = { 0xF8, 0xF0, 0x05, 0x94, 0x3D, 0x8B };
int wifiStatus = WL_IDLE_STATUS;
int off = 0;
int red = 1;
int green = 2;
int blue = 3;
int yellow = 4;
int flashInterval = 100; // ms


void setup() {
  Ethernet.init(12); // Avoid conflict with LED pins
  initDebug();
  debug("Starting");
  initLED();
  flashLED(blue, 1);
  initEthernet();
  flashLED(green, 2);
  initWiFi();
  flashLED(green, 3);
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

void initEthernet() {
  while (Ethernet.begin(mac) != 1)
  {
    flashLED(red, 2);
    debug("Failed to start ethernet");
    delay(2000);
  }
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
    flashLED(yellow, 2);
    delay(5000);
  }

  debug("Connected to WiFi");
}

void initLED() {
  PORT->Group[PORTA].DIRSET.reg = RGB;
}

void flashLED(int color, int count) {
  for (int i = 0; i < count; i++) {
    setLED(color);
    delay(flashInterval);
    setLED(off);
    if (i < count - 1) {
      delay(flashInterval);
    }
  }
}

void setLED(int color)
{
  PORT->Group[PORTA].OUTCLR.reg = RGB;
  if (color == red) {
    PORT->Group[PORTA].OUTSET.reg = RED;
  } else if (color == green) {
    PORT->Group[PORTA].OUTSET.reg = GRN;
  } else if (color == blue) {
    PORT->Group[PORTA].OUTSET.reg = BLU;
  } else if (color == yellow) {
    PORT->Group[PORTA].OUTSET.reg = RED | GRN;
  }
}

float getVoltage() {
  int sensorValue = analogRead(A0);
  return float(sensorValue) * (BOARD_VOLTAGE / 1024.0) / (R2 / (R1 + R2));
}

void displayVoltage(float voltage) {
  if (voltage < FULL_CHARGE * 0.6) {
    flashLED(red, 2);
  } else if (voltage < FULL_CHARGE * 0.8) {
    flashLED(yellow, 2);
  } else {
    flashLED(green, 2);
  }
  debug(voltage);
}

void reportVoltage(float voltage) {
  debug("TODO: Report voltage 2");
  return;

  EthernetClient c;
  HttpClient http(c);

  int err = 0;

  err = http.get("www.arduino.cc", "/asciilogo.txt");
  if (err == 0)
  {
    debug("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);
    }
  }


  //  HttpClient client;
  //  client.get("http://www.arduino.cc/asciilogo.txt");
  //  while (client.available()) {
  //    char c = client.read();
  //    Serial.print(c);
  //  }
  //  Serial.flush();
}
