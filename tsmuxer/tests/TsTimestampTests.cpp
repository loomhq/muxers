
#include <gtest/gtest.h>

#include "include/TsMuxer.h"

namespace {

static int64_t decodeTimestamp(uint8_t *x) {
  int64_t pts = ((x[0] >> 1) & 0x7ULL) << 30;

  int z = ((x[1] << 8 | x[2]) >> 1);
  pts |= (z << 15);

  int y = (x[3] << 8 | x[4]) >> 1;
  pts |= y;

  return pts;
}

// exhaustively test the timestamp encoder for all 2^33 possible transport stream timestamps,
// including "negative" ones, so 2^34.
TEST(TsTimestampTest, testAllTimestampsSurviveRoundTrip) {
  uint8_t x[5];
  const int64_t kMaxPts = 1LL << 33;

  for (int64_t pts = 0; pts < kMaxPts; pts++) {
    // so people don't get bored
    if ((pts % (kMaxPts / 10)) == 0) std::cout << "testing pts at " << (100*pts/kMaxPts) << "%" << std::endl;
    writeTsMuxerPts(x, 0b0010, pts);

    const int64_t decodedPts = decodeTimestamp(x);

    ASSERT_TRUE(decodedPts == pts);
  }
}

}  // namespace
