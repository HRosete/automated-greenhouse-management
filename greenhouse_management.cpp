#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <dht.h>

// Pin Definitions
#define DHT22_PIN1 2
#define DHT22_PIN2 3
#define readmoistd1 37
#define readmoistd2 36
#define VALVE 44
#define L1 42
#define L2 43
#define L3 45
#define stepm 4
#define dirm 35

const int FAN_PINS[] = {41, 40, 39, 38};
const int MOISTURE_PINS[] = {readmoistd1, readmoistd2};
const int LDR_PINS[] = {A0, A1};
const int MOISTURE_ANALOG_PINS[] = {A2, A3};

// Variables
float hum[2];
float temp[2];
int delay_1 = 10;
int delay_2 = 1000;
int margin_3 = 100;
int margin_4 = 10;

void setup() {
  Serial.begin(9600);
  pinMode(stepm, OUTPUT);
  pinMode(dirm, OUTPUT);

  for (int i = 0; i < sizeof(FAN_PINS) / sizeof(FAN_PINS[0]); i++) {
    pinMode(FAN_PINS[i], OUTPUT);
  }

  pinMode(VALVE, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
}

void loop() {
  int h = hour();
  int chk1 = DHT.read22(DHT22_PIN1);
  hum[0] = DHT.humidity;
  temp[0] = DHT.temperature;

  int chk2 = DHT.read22(DHT22_PIN2);
  hum[1] = DHT.humidity;
  temp[1] = DHT.temperature;

  for (int i = 0; i < sizeof(LDR_PINS) / sizeof(LDR_PINS[0]); i++) {
    int sensorValue = analogRead(LDR_PINS[i]);
    float sensorVoltage = sensorValue * (5.0 / 1024.0);
    float resistance = (sensorVoltage * 5000.0) / (5.0 - sensorVoltage);

    if ((h >= 6 && h < 11) || (h >= 17 && h < 22)) {
      controlLights(h, resistance, resistance, L1 + i);
    }

    if (h >= 6 && h < 11) {
      ValveOn(h, MOISTURE_ANALOG_PINS[i]);
    }

    if (h >= 11 && h < 16 && (temp[0] >= 30 || temp[1] >= 30)) {
      OnFanLines();
      MoveStepper1();
      OffFanLines();
    } else {
      OnFanLines();
      MoveStepper2();
      OffFanLines();
    }
  }

  printValues();
  delay(5000);
}

void controlLights(int hour, float resistance1, float resistance2, int pin) {
  if ((hour >= 17 && hour < 19) || (hour < 17 && (resistance1 > 400 || resistance2 > 400))) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}

void ValveOn(int hour, int analogPin) {
  if ((hour == 6 || hour == 17) && analogRead(analogPin) >= 570) {
    digitalWrite(VALVE, HIGH);
  } else {
    digitalWrite(VALVE, LOW);
  }
}

void MoveStepper1() {
  digitalWrite(dirm, HIGH);
  for (int x = 0; x < margin_4; x++) {
    for (int x = 0; x < margin_3; x++) {
      digitalWrite(stepm, HIGH);
      delayMicroseconds(delay_1);
      digitalWrite(stepm, LOW);
      delayMicroseconds(delay_1);
    }
  }
}

void MoveStepper2() {
  digitalWrite(dirm, LOW);
  for (int x = 0; x < margin_4; x++) {
    for (int x = 0; x < margin_3; x++) {
      digitalWrite(stepm, HIGH);
      delayMicroseconds(delay_1);
      digitalWrite(stepm, LOW);
      delayMicroseconds(delay_1);
    }
  }
}

void OnFanLines() {
  for (int i = 0; i < sizeof(FAN_PINS) / sizeof(FAN_PINS[0]); i++) {
    digitalWrite(FAN_PINS[i], HIGH);
  }
}

void OffFanLines() {
  for (int i = 0; i < sizeof(FAN_PINS) / sizeof(FAN_PINS[0]); i++) {
    digitalWrite(FAN_PINS[i], LOW);
  }
}

void printValues() {
  Serial.print(hum[0]);
  Serial.print("\t");
  Serial.print(temp[0]);
  Serial.print("\t");
  Serial.print(hum[1]);
  Serial.print("\t");
  Serial.print(temp[1]);
  Serial.print("\t");

  for (int i = 0; i < sizeof(LDR_PINS) / sizeof(LDR_PINS[0]); i++) {
    int sensorValue = analogRead(LDR_PINS[i]);
    float sensorVoltage = sensorValue * (5.0 / 1024.0);
    float resistance = (sensorVoltage * 5000.0) / (5.0 - sensorVoltage);
    Serial.print(sensorVoltage);
    Serial.print("\t");
    Serial.print(resistance);
    Serial.print("\t");
  }

  for (int i = 0; i < sizeof(MOISTURE_ANALOG_PINS) / sizeof(MOISTURE_ANALOG_PINS[0]); i++) {
    Serial.print(analogRead(MOISTURE_ANALOG_PINS[i]));
    Serial.print("\t");
    Serial.print(digitalRead(MOISTURE_PINS[i]));
    Serial.print("\t");
  }
  
  Serial.println();
}
