#include <gtest/gtest.h>

#include <thread>
#include <vector>

#include "TsMuxerUtils.h"

namespace {

static int64_t decodeTimestamp(uint8_t *x) {
  int64_t pts = ((x[0] >> 1) & 0x7ULL) << 30;

  int z = ((x[1] << 8 | x[2]) >> 1);
  pts |= (z << 15);

  int y = (x[3] << 8 | x[4]) >> 1;
  pts |= y;

  return pts;
}

// exhaustively test the timestamp encoder for all 2^33 possible transport
// stream timestamps
TEST(TsMuxerUtilsTest, testAllTimestampsSurviveRoundTrip) {
  const int64_t kMaxPts = 1LL << 33;

  const int num_threads = 10;
  const int64_t batch_size = kMaxPts / num_threads;

  std::vector<std::thread> threads;

  for (int thread_num = 0; thread_num < num_threads; thread_num++) {
    const int64_t start_pts = thread_num * batch_size;
    int64_t end_pts = (thread_num + 1) * batch_size + 1;

    if (thread_num == num_threads - 1) {
      end_pts = kMaxPts;
    }
    auto f = [=]() {
      uint8_t x[5];
      for (int64_t pts = start_pts; pts < end_pts; pts++) {
        TsMuxerUtils::writePts(x, 0b0010, pts);

        const int64_t decodedPts = decodeTimestamp(x);

        ASSERT_EQ(decodedPts, pts);
      }
    };
    std::thread thread(f);
    threads.push_back(std::move(thread));
  }

  // Iterate over the thread vector
  for (auto &thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

}  // namespace
