#include <SPI.h>
#include <WiFi101.h>
#include "Config.h"
#include "LED.h"
#include "Logger.h"

byte mac[] = {0xF8, 0xF0, 0x05, 0x94, 0x3D, 0x8B};
int wifiStatus = WL_IDLE_STATUS;
Logger logger;
LED led;
int flashInterval = 200;

WiFiClient client;

void setup()
{
  logger.init();
  logger.print("Starting");

  // Blue light at startup
  led.init(LED_REG_R, LED_REG_G, LED_REG_B, flashInterval);
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
  delay(2000);
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
  char route[80];
  sprintf(route, "POST %s HTTP/1.1", REMOTE_PATH);
  char data[80];
  sprintf(data, "{\"voltage\":%f}", voltage);

  logger.print("Attempting to send");
  logger.print(data);

  if (client.connectSSL(REMOTE_HOST, REMOTE_PORT))
  {
    client.println(route);
    client.print("host: ");
    client.println(REMOTE_HOST);
    client.print("x-api-key: ");
    client.println(REMOTE_API_KEY);
    client.println("connection: close");
    client.println("content-type: application/json");
    client.print("content-length: ");
    client.println(strlen(data));
    client.println();
    client.print(data);

    delay(1000);

#ifdef DEBUG
    while (client.available())
    {
      char c = client.read();
      Serial.write(c);
    }
#endif

    if (client.connected()) { 
      client.stop();
    }

    led.sequence(led.BLUE, led.GREEN);
  } else {
    led.sequence(led.BLUE, led.RED);
  }
}
