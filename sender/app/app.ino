#include <LiquidCrystal.h>

// https://cdn-shop.adafruit.com/product-files/181/p181.pdf
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Specify the resistors and board voltage to use
const float r1 = 1000000, r2 = 220000;
const float boardVoltage = 5.0;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Starting...");
}

void loop() {
  int sensorValue = analogRead(A0);
  float voltage = calcVoltage(sensorValue);
  displayVoltage(voltage);
  delay(1000);
}

float calcVoltage(float sensorValue) {
  return sensorValue * (boardVoltage / 1023.0) / (r2 / (r1 + r2));
}

void displayVoltage(float voltage) {
  lcd.setCursor(0, 0);
  lcd.print("Voltage:            ");
  lcd.setCursor(9, 0);
  lcd.print(voltage);
}

void reportVoltage(float voltage) {
  // todo: call remote api
}
