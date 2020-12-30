#include <SPI.h>
#include <WiFi101.h>
#include "Config.h"
#include "LED.h"
#include "Logger.h"

int wifiStatus = WL_IDLE_STATUS;
Logger logger;
LED led;
int flashInterval = 200;

WiFiSSLClient client;

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

#ifdef DEBUG
  printWifiStatus();
#endif

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
  char msg[80];
  sprintf(msg, "Attempting to connect to %s", WIFI_SSID);
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

  char dbg[80];
  sprintf(dbg, "Read voltage %f", voltage);
  logger.print(dbg);
}

void reportVoltage(float voltage)
{
  uint8_t max_retries = 0;
  while (true)
  {
    client.connect(REMOTE_HOST, REMOTE_PORT);

    if (client.connected())
    {
      break;
    }

    led.sequence(led.BLUE, led.RED);

    if (++max_retries > 5)
    {
      logger.print("Could not connect to server");
      return;
    }

    delay(2000);
  }

  char body[30];
  sprintf(body, "{\"voltage\":%f}", voltage);

  char request[1000];
  sprintf(request, "POST %s HTTP/1.1\r\n", REMOTE_PATH);
  sprintf(request, "%sconnection: close\r\n", request);
  sprintf(request, "%scontent-length: %u\r\n", request, strlen(body));
  sprintf(request, "%scontent-type: application/json\r\n", request);
  sprintf(request, "%shost: %s\r\n", request, REMOTE_HOST);
  sprintf(request, "%sx-api-key: %s\r\n", request, REMOTE_API_KEY);
  sprintf(request, "%s\r\n%s", request, body);
  client.print(request);

  logger.print(request);

  delay(1000);

#ifdef DEBUG
  while (client.available())
  {
    char c = client.read();
    Serial.write(c);
  }
#endif

  client.stop();
  led.sequence(led.BLUE, led.GREEN);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("Successfully Connected to Network with SSID: ");
  Serial.print(WiFi.SSID());

  String fv = WiFi.firmwareVersion();
  Serial.print("  Firmware Version: ");
  Serial.println(fv);

  // print Board's Local IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("  Local IP Address: ");
  Serial.println(ip);

  // print Board's Gateway IP address:
  IPAddress gwip = WiFi.gatewayIP();
  Serial.print("  Gateway IP Address: ");
  Serial.println(gwip);
 
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("  signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");

  int encryptionType = WiFi.encryptionType();
  Serial.print("  encryption type: ");
  Serial.print(encryptionType);
  Serial.println(" (TKIP (WPA)=2,WEP=5,CCMP (WPA)=4,NONE=7,AUTO=8)"); 
}