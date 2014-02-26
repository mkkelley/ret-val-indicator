#include "Arduino.h"
#define RET 0

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
