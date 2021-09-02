
#include "tsmuxer/tsmuxer.h"

#include "gtest/gtest.h"

namespace {

class TsMuxerTest : public ::testing::Test {
 protected:
  void SetUp() override { ts_muxer_ = createTsMuxer(); }

  void TearDown() override { destroyTsMuxer(ts_muxer_); }

  struct TsMuxer* ts_muxer_ = nullptr;
};

TEST_F(TsMuxerTest, shouldSucceed_whenCallingCreateAndDestroy) {
  ASSERT_TRUE(ts_muxer_ != nullptr);
}

}  // namespace
