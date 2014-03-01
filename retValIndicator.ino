#include "tx.h"
#define SUCCESS_LED_PIN 2
#define FAILURE_LED_PIN 3
#define OFF_SWITCH_PIN 23
#define CLEAR_SWITCH_PIN 25
#define DEBUG true

boolean is_switch_on() {
  int buttonState = digitalRead(OFF_SWITCH_PIN);
  return buttonState == LOW;
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

void write_debug_info() {
  Serial.write("so:");
  if (is_switch_on()) {
    Serial.write("y");
  } else {
    Serial.write("n");
  }
  Serial.write(" cs:");
  if (is_clear_switch_on()) {
    Serial.write("y");
  } else {
    Serial.write("n");
  }
  Serial.write('\n');
}

void blink(int led, int times, int udelay) {
  for (int i = 0; i < times; ++i) {
    digitalWrite(led, HIGH);
    delay(udelay);
    digitalWrite(led, LOW);
    delay(udelay);
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
  if (!is_switch_on()) {
    clear_lights();
    clearSerialBuffer();
    digitalWrite(13, LOW);
    return;
  } else {
    digitalWrite(13, HIGH);
  }

  if (DEBUG) {
    write_debug_info();
  }

  if (is_clear_switch_on()) {
    clear_lights();
  }

  Transmission tx; //blocks until 4 bytes read

  byte ret_val;
  switch (tx.type()) {
  case TX_RET:
    ret_val = get_ret_tx_val(tx);
    if (ret_val == 0) {
      light_success();
    } else {
      light_failure();
    }
    break;
  case TX_CLEAR:
    clear_lights();
    break;
  default:
    blink(13, 4, 100);
    clearSerialBuffer();
  }
}
