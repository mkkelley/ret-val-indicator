#include "tx.h"

tx_t tx_type(byte tx[]) {
  return tx[0] == 'R' && tx[1] == 'E' && tx[2] == 'T';
}

byte get_ret_tx_val(byte tx[]) {
  return tx[3];
}
