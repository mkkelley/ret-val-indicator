#include "tx.h"

byte get_ret_tx_val(Transmission tx) {
  return tx.tx[3];
}

Transmission::Transmission() {
  for (byte i = 0; i < 4; i++) {
    tx[i] = Serial.read();
  }
}

tx_t Transmission::type() const {
  if (tx[0] == 'R' && tx[1] == 'E' && tx[2] == 'T') {
    return RET;
  }
}
