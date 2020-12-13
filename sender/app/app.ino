// #include <b64.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <SPI.h>
#include <WiFi101.h>
#include "LED.h"
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

// LED pins
#define LED_PIN_R 9
#define LED_PIN_G 10
#define LED_PIN_B 11

byte mac[] = { 0xF8, 0xF0, 0x05, 0x94, 0x3D, 0x8B };
int wifiStatus = WL_IDLE_STATUS;
LED led(LED_PIN_R, LED_PIN_G, LED_PIN_B);

void setup() {
  Ethernet.init(12); // Avoid conflict with LED pins
  initDebug();
  debug("Starting");
  led.flash(led.BLUE, 1);
  initEthernet();
  led.flash(led.BLUE, 2);
  initWiFi();
  led.flash(led.GREEN, 3);
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
    led.flash(led.RED, 2);
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
    led.flash(led.RED | led.GREEN, 2);
    delay(5000);
  }

  debug("Connected to WiFi");
}

float getVoltage() {
  int sensorValue = analogRead(A0);
  return float(sensorValue) * (BOARD_VOLTAGE / 1024.0) / (R2 / (R1 + R2));
}

void displayVoltage(float voltage) {
  if (voltage < FULL_CHARGE * 0.6) {
    led.flash(led.RED, 2);
  } else if (voltage < FULL_CHARGE * 0.8) {
    led.flash(led.RED | led.GREEN, 2);
  } else {
    led.flash(led.GREEN, 2);
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
