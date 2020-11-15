const float fullCharge = 12.8;
const int ledPin = 13;

// Specify the resistors and board voltage to use
const float r1 = 1000000, r2 = 220000;
const float boardVoltage = 5.0;

void setup() {
  debug("Starting");
  initDisplay();
  initLED();
}

void loop() {
  int sensorValue = analogRead(A0);
  float voltage = calcVoltage(sensorValue);
  displayVoltage(voltage);
  delay(2000);
}

void debug(String text) {
  if (Serial) Serial.println(text);
}

void initLED() {
  pinMode(ledPin, OUTPUT);
}

void setLED(bool on) {
  digitalWrite(ledPin, on ? HIGH : LOW);
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

void initDisplay() {
  if (!Serial) return;
  Serial.begin(9600);
}

float calcVoltage(float sensorValue) {
  return sensorValue * (boardVoltage / 1023.0) / (r2 / (r1 + r2));
}

void displayVoltage(float voltage) {
  if (voltage < fullCharge * 0.6) {
    flashLED(3);
  } else if (voltage < fullCharge * 0.8) {
    flashLED(2);
  } else {
    flashLED(1);
  }
  if (Serial) Serial.println(voltage);
}

void reportVoltage(float voltage) {
  // todo: call remote api
}
