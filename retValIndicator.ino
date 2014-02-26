#include "tx.h"
#define SUCCESS_LED_PIN 2
#define FAILURE_LED_PIN 3
#define SWITCH_PIN 12

boolean is_switch_on() {
  int buttonState = digitalRead(SWITCH_PIN);
  if (buttonState == HIGH) {
    return true;
  }
  return false;
}

void light_failure() {
  digitalWrite(SUCCESS_LED_PIN, LOW);
  digitalWrite(FAILURE_LED_PIN, HIGH);
}

void light_success() {
  digitalWrite(FAILURE_LED_PIN, LOW);
  digitalWrite(SUCCESS_LED_PIN, HIGH);
}

void clearSerialBuffer() {
  while (Serial.available()) {
    Serial.read();
  }
}

void setup() {
  pinMode(SUCCESS_LED_PIN, OUTPUT);
  pinMode(FAILURE_LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (!is_switch_on()) {
    clearSerialBuffer();
    delay(100);
    return;
  }
  
  byte transmission[4];
  for (byte i = 0; i < 4; i++) {
    transmission[i] = Serial.read();
  }
  
  if (tx_type(transmission)) {
    byte ret_val = get_ret_tx_val(transmission);
    if (ret_val == 0) {
      light_success();
    } else {
      light_failure();
    }
  }
}
