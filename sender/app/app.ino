// #include <b64.h>
#include <Ethernet.h>
// #include <HttpClient.h>
#include <SPI.h>
#include <WiFi101.h>
#include "Config.h"
#include "LED.h"
#include "Logger.h"

byte mac[] = {0xF8, 0xF0, 0x05, 0x94, 0x3D, 0x8B};
int wifiStatus = WL_IDLE_STATUS;
Logger logger;
LED led;

WiFiClient client;

void setup()
{
  logger.init();
  logger.print("Starting");

  // Ethernet.init(12); // Avoid conflict with LED pins

  led.init(LED_REG_R, LED_REG_G, LED_REG_B);
  led.flash(led.BLUE, 3);

  // led.flash(led.RED, 1);
  // delay(500);
  // led.flash(led.GREEN, 1);
  // delay(500);
  // led.flash(led.BLUE, 1);
  // delay(2000);

  initWiFi();
  while (!connectWiFi())
  {
    led.flash(led.RED, 3);
    delay(2000);
  }

  led.flash(led.GREEN, 3);
}

void loop()
{
  float voltage = getVoltage();
  displayVoltage(voltage);
  reportVoltage(voltage);
  delay(CHECK_INTERVAL);
}

void initWiFi()
{
  logger.print("Initializing WiFi");
  WiFi.setPins(WIFI_CHIPSELECT, WIFI_IRQ, WIFI_RESET, WIFI_ENABLE);

  if (WiFi.status() == WL_NO_SHIELD)
  {
    logger.print("WiFi shield not present");
    while (true)
      ;
  }
  logger.print("WiFi shield detected");
}

bool connectWiFi()
{
  char msg[50];
  strcpy(msg, "Attempting to connect to ");
  strcat(msg, WIFI_SSID);
  logger.print(msg);
  if (WL_CONNECTED == WiFi.begin(WIFI_SSID, WIFI_PASS))
  {
    logger.print("Connected to WiFi");
    return true;
  }
  return false;
}

float getVoltage()
{
  int sensorValue = analogRead(A0);
  return float(sensorValue) * (BOARD_VOLTAGE / 1024.0) / (R2 / (R1 + R2));
}

void displayVoltage(float voltage)
{
  if (voltage < FULL_CHARGE * 0.6)
  {
    led.flash(led.RED, 2);
  }
  else if (voltage < FULL_CHARGE * 0.8)
  {
    led.flash(led.RED | led.GREEN, 2);
  }
  else
  {
    led.flash(led.GREEN, 2);
  }
  logger.print(voltage);
}

void reportVoltage(float voltage)
{
  logger.print("TODO: Report voltage 2");
  return;

  // EthernetClient c;
  // HttpClient http(c);

  // int err = 0;

  // err = http.get("www.arduino.cc", "/asciilogo.txt");
  // if (err == 0)
  // {
  //   logger.print("startedRequest ok");

  //   err = http.responseStatusCode();
  //   if (err >= 0)
  //   {
  //     Serial.print("Got status code: ");
  //     Serial.println(err);
  //   }
  // }

  //  HttpClient client;
  //  client.get("http://www.arduino.cc/asciilogo.txt");
  //  while (client.available()) {
  //    char c = client.read();
  //    Serial.print(c);
  //  }
  //  Serial.flush();
}
