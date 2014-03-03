#include "tx.h"
#define BOUNCE_DURATION 200
#define SUCCESS_LED_PIN 4
#define FAILURE_LED_PIN 5
#define OFF_SWITCH_PIN 23
// Interrupt 0 is on pin 2
#define CLEAR_SWITCH_INTERRUPT 0
#define DEBUG true

boolean is_switch_on() {
  int buttonState = digitalRead(OFF_SWITCH_PIN);
  return buttonState == LOW;
}

void light_failure() {
  digitalWrite(SUCCESS_LED_PIN, LOW);
  digitalWrite(FAILURE_LED_PIN, HIGH);
}

void light_success() {
  digitalWrite(FAILURE_LED_PIN, LOW);
  digitalWrite(SUCCESS_LED_PIN, HIGH);
}

// The clear button WILL BREAK if this modifies non-volatile
// variables. (see clearHandler())
void clear_lights() {
  digitalWrite(FAILURE_LED_PIN, LOW);
  digitalWrite(SUCCESS_LED_PIN, LOW);
}

void clearSerialBuffer() {
  while (Serial.available()) {
    Serial.read();
  }
}

volatile static unsigned long bounceTime = 0;

void clearHandler() {
  if (abs(millis() - bounceTime) < BOUNCE_DURATION) {
    return;
  }
  // This WILL BREAK if clear_lights ever modifies any variables
  clear_lights();
  bounceTime = millis();
}

void write_debug_info() {
  Serial.write("so:");
  if (is_switch_on()) {
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
  pinMode(13, OUTPUT);
  attachInterrupt(CLEAR_SWITCH_INTERRUPT, clearHandler, RISING);
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
