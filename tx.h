#include "Arduino.h"
#define TX_GARBLED 0
#define TX_RET 1
#define TX_CLEAR 2

typedef int tx_t;

class Transmission {
public:
  byte tx[4];
  /*
   * Get the next transmission, block until 4 bytes received.
   */
  Transmission();
  tx_t type() const;
};

// use for tx_t == RET
byte get_ret_tx_val(Transmission);
