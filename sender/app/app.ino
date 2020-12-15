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

  // Blue light at startup
  led.init(LED_REG_R, LED_REG_G, LED_REG_B);
  led.flash(led.BLUE, 3);

  // Red light while wifi is connecting
  initWiFi();
  while (!connectWiFi())
  {
    led.flash(led.RED, 3);
    delay(2000);
  }

  // Green when ready to go
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
  // if (client.connect("www.google.com", 80))
  // {
  //   Serial.println("connected to server");
  //   client.println("GET /search?q=arduino HTTP/1.1");
  //   client.println("Host: www.google.com");
  //   client.println("Connection: close");
  //   client.println();

  //   delay(1000);

  //   while (client.available())
  //   {
  //     char c = client.read();
  //     Serial.write(c);
  //   }
  // }
}
