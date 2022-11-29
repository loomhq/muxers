#include "TsMuxerUtils.h"

// Will write 5 bytes
void TsMuxerUtils::writePts(uint8_t* q, uint8_t four_bits, int64_t pts) {
  int val;
  val = (int)(four_bits << 4 | (((pts >> 30) & 0x07) << 1) | 1);
  *q++ = val;
  val = (int)((((pts >> 15) & 0x7fff) << 1) | 1);
  *q++ = val >> 8;
  *q++ = val;
  val = (int)((((pts)&0x7fff) << 1) | 1);
  *q++ = val >> 8;
  *q++ = val;
}
