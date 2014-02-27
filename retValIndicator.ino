#include "tx.h"
#define SUCCESS_LED_PIN 2
#define FAILURE_LED_PIN 3
#define OFF_SWITCH_PIN 23
#define CLEAR_SWITCH_PIN 25

boolean is_switch_on() {
  int buttonState = digitalRead(OFF_SWITCH_PIN);
  return buttonState == HIGH;
}

boolean is_clear_switch_on() {
  int buttonState = digitalRead(CLEAR_SWITCH_PIN);
  return buttonState == HIGH;
}

void light_failure() {
  digitalWrite(SUCCESS_LED_PIN, LOW);
  digitalWrite(FAILURE_LED_PIN, HIGH);
}

void light_success() {
  digitalWrite(FAILURE_LED_PIN, LOW);
  digitalWrite(SUCCESS_LED_PIN, HIGH);
}

void clear_lights() {
  digitalWrite(FAILURE_LED_PIN, LOW);
  digitalWrite(SUCCESS_LED_PIN, LOW);
}

void clearSerialBuffer() {
  while (Serial.available()) {
    Serial.read();
  }
}

void setup() {
  pinMode(SUCCESS_LED_PIN, OUTPUT);
  pinMode(FAILURE_LED_PIN, OUTPUT);
  pinMode(OFF_SWITCH_PIN, INPUT);
  pinMode(CLEAR_SWITCH_PIN, INPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.write("so:");
  if (is_switch_on()) {
    Serial.write("y");
  } else {
    Serial.write("n");
  }
  Serial.write("cs:");
  if (is_clear_switch_on()) {
    Serial.write("y");
  } else {
    Serial.write("n");
  }
  if (!is_switch_on()) {
    clear_lights();
    clearSerialBuffer();
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    return;
  }
  
  if (is_clear_switch_on()) {
    clear_lights();
  }

  Transmission tx; //blocks until 4 bytes read
  
  if (tx.type() == TX_RET) {
    byte ret_val = get_ret_tx_val(tx);
    if (ret_val == 0) {
      light_success();
    } else {
      light_failure();
    }
  }
}
