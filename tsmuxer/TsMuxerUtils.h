#ifndef TSMUXERUTILS_H_HEADER
#define TSMUXERUTILS_H_HEADER

#include <stdint.h>

#include <vector>

namespace TsMuxerUtils {

void writePts(uint8_t* q, uint8_t four_bits, int64_t pts);

}  // namespace TsMuxerUtils

#endif  // TSMUXERUTILS_H_HEADER