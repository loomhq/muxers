
#include <tsmuxer/include/TsMuxer.h>

#include <gtest/gtest.h>

#include "include/TsMuxer.h"

static const uint8_t kH264IntraFrameWithSPSAndPPS[] = {
    0x00, 0x00, 0x00, 0x01, 0x67, 0x64, 0x00, 0x0A, 0xAC, 0xD9, 0x4C, 0x9E,
    0x59, 0x7F, 0xF0, 0x11, 0x10, 0x10, 0xF6, 0xA0, 0x20, 0x20, 0xC8, 0x00,
    0x00, 0x03, 0x00, 0x08, 0x00, 0x00, 0x03, 0x01, 0xE0, 0x78, 0x91, 0x2C,
    0xB0, 0x00, 0x00, 0x00, 0x01, 0x68, 0xEB, 0xE3, 0xCB, 0x22, 0xC0, 0x00,
    0x00, 0x01, 0x65, 0x88, 0x84, 0x01, 0xFF, 0xCC, 0x15, 0xE1, 0x5F, 0x8B,
    0x42, 0x3B, 0x57, 0xBB, 0xF3, 0x48, 0x10, 0x5B, 0xBE, 0xE7, 0x6B, 0x4E,
    0x6F, 0xA3, 0x95, 0x9A, 0xEA, 0x4A, 0x7D, 0xD9, 0x3E, 0xDC, 0x5A, 0x9D,
    0x2B, 0x86, 0x1E, 0xEC, 0xC0, 0xC8, 0xC6, 0x0F, 0xAC, 0xFB, 0x10, 0x23,
    0x7B, 0x1C, 0xAF, 0xEC, 0xE6, 0x6E, 0xA2, 0x42, 0xFC, 0x3E, 0x13, 0xF3,
    0x45, 0x74, 0x07, 0x43, 0xE3, 0xBF, 0x11, 0x49, 0xE4, 0x20, 0xE8, 0x7C,
    0x87, 0x11, 0xB2, 0x9B, 0x37, 0x86, 0x7E, 0x78, 0x61, 0xDD, 0x8A, 0xB4,
    0xB1, 0x17, 0x93, 0x46, 0xBB, 0x80, 0x9F, 0x73, 0x4F, 0xB2, 0x81, 0xB7,
    0xE1, 0xCF, 0xB7, 0xD6, 0x92, 0x15, 0xD7, 0xD7, 0x83, 0xBC, 0x3F, 0xC6,
    0xFE, 0x07, 0x90, 0xF7, 0x7B, 0xDB, 0xF1, 0x05, 0x67, 0xD4, 0x85, 0x18,
    0x2E, 0x94, 0x68, 0x40, 0xE1, 0x41, 0xD7, 0x03, 0xEC, 0x09, 0xBA, 0x51,
    0x9D, 0x9A, 0xCD, 0x9B, 0xB2, 0x5A, 0xF7, 0x3C, 0x42, 0xCD, 0x66, 0xA6,
    0xF9, 0x8B, 0xEB, 0x2E, 0xF3, 0x55, 0x5C, 0x31, 0x06, 0x74, 0xD1, 0xED,
    0x4A, 0x17, 0xB6, 0x63, 0x25, 0x96, 0xB5, 0xE9, 0x6D, 0x25, 0x2A, 0x9E,
    0xAF, 0x42, 0xD1, 0x7F, 0x2F, 0x70, 0x1E, 0x62, 0x47, 0x99, 0xE5, 0x12,
    0x5E, 0xE3, 0x7A, 0x4B, 0x2A, 0x3D, 0xBA, 0xCE, 0x8D, 0x9B, 0x1F, 0xEC,
    0x82, 0x74, 0xF0, 0xE6, 0x1D, 0x72, 0x74, 0xEE, 0x64, 0x27, 0x03, 0xDB,
    0xBA, 0x48, 0x6B, 0xBF, 0x45, 0xAC, 0x74, 0xD3, 0x15, 0x38, 0xF6, 0xBD,
    0x63, 0x8E, 0x92, 0x12, 0x5F, 0xDA, 0x24, 0xD2, 0xC1, 0x54, 0x96, 0x1D,
    0xD9, 0x07, 0x7A, 0x66, 0x4E, 0xED, 0xD5, 0x56, 0x6D, 0x13, 0x40, 0x77,
    0x0B, 0x16, 0xB0, 0x4C, 0x03, 0xA0, 0x69, 0x5D, 0x26, 0x09, 0x8D, 0x0F,
    0xCC, 0x50, 0x1C, 0xAB, 0x6C, 0x7E, 0x1C, 0x45, 0xCA, 0xB6, 0x0D, 0x3D,
    0x43, 0x74, 0x48, 0xB2, 0x1E, 0x70, 0x66, 0x5C, 0x45, 0x89, 0x0E, 0x74,
    0x47, 0x25, 0x4B, 0x2D, 0x91, 0x81, 0xA3, 0xB1, 0x41, 0x5C, 0x5A, 0x89,
    0x73, 0x38, 0x0F, 0xB7, 0x6A, 0x8E, 0x1A, 0x0D, 0x05, 0x16, 0x0D, 0x40,
    0xFE, 0x99, 0xAB, 0x92, 0x7A, 0xA2, 0x5F, 0xA4, 0x21, 0x28, 0xC3, 0x21,
    0xD4, 0x51, 0x43, 0x18, 0x4D, 0xEF, 0x76, 0xCF, 0x67, 0x6A, 0x98, 0xA5,
    0xDC, 0xFE, 0x83, 0x2F, 0x40, 0x02, 0x58, 0x59, 0x8F, 0x82, 0xCD, 0xC5,
    0x84, 0xFA, 0x96, 0x2C, 0x00, 0x9A, 0x8A, 0x12, 0xFF, 0x7A, 0x9E, 0x0E,
    0xF8, 0xF4, 0xDC, 0xFF, 0xCF, 0x66, 0xD4, 0x8D, 0xDD, 0x9D, 0xFC, 0x30,
    0x87, 0xE4, 0xA9, 0x94, 0x28, 0xDE, 0x28, 0x53, 0x29, 0xA1, 0x7D, 0xCB,
    0x70, 0x41, 0x4C, 0xF8, 0xB9, 0x74, 0x6D, 0xA7, 0xD8, 0x2D, 0x03, 0x0D,
    0x0C, 0x62, 0x93, 0x06, 0xB8, 0xC4, 0x22, 0x1C, 0x9C, 0x99, 0x14, 0x60,
    0xE2, 0x7F, 0x21, 0x2C, 0x0A, 0x57, 0x11, 0x2E, 0xAC, 0x12, 0x83, 0x2D,
    0xA6, 0x24, 0x5B, 0xAA, 0x7B, 0x4F, 0x0A, 0x7F, 0x46, 0x57, 0x02, 0x8E,
    0xEE, 0x9C, 0xA9, 0x1C, 0x3F, 0x3C, 0xDB, 0x25, 0xA8, 0x86, 0x7B, 0x07,
    0xA4, 0xAF, 0x10, 0x2D, 0x71, 0xEF, 0x10, 0x3E, 0xA4, 0xDF, 0xFA, 0xA3,
    0x01, 0xEB, 0x1E, 0x89, 0xC4, 0x98, 0x66, 0x55, 0xE7, 0xB3, 0x09, 0xA5,
    0x87, 0x4B, 0x40, 0xC8, 0xCF, 0xA6, 0x01, 0x06, 0x4D, 0x69, 0xF8, 0x35,
    0x94, 0xAA, 0xCF, 0xB2, 0x2F, 0x45, 0x17, 0xD5, 0x7E, 0xA5, 0x4C, 0x45,
    0xB4, 0x7B, 0xCD, 0x37, 0xDE, 0x7A, 0xE5, 0x5A, 0xAC, 0xE3, 0x41, 0x12,
    0x54, 0xDE, 0xA7, 0xF2, 0x0B, 0x64, 0xF7, 0x8A, 0x95, 0x53, 0xFF, 0x4D,
    0x98, 0xE1, 0xE3, 0x31, 0x10, 0xBF, 0x39, 0x1A, 0x09, 0x32, 0xD7, 0x6B,
    0xAB, 0x75, 0xE5, 0x97, 0xA8, 0x17, 0xBF, 0xFE, 0x5D, 0x08, 0x27, 0x78,
    0x27, 0x5E, 0xB1, 0xB1, 0xA8, 0x09, 0x9C, 0xAE, 0x23, 0xC4, 0x59, 0x56,
    0x08, 0xE1, 0x38, 0x3B, 0xC4, 0x66, 0xF6, 0x8D, 0x0F, 0x03, 0xEE, 0x56,
    0x26, 0xDD, 0x4E, 0x83, 0x2A, 0x24, 0xA1, 0xC3, 0x5C, 0x5B, 0x81, 0x36,
    0x59, 0x6D, 0xE5, 0xD3, 0x76, 0x26, 0x34, 0x68, 0x18, 0x2D, 0x0B, 0x9A,
    0x15, 0x90, 0xEA, 0x34, 0x80, 0xDA, 0xA4, 0xE3, 0x56, 0xAE, 0x1B, 0xBE,
    0xE7, 0xB4, 0x1C, 0xDC, 0x56, 0x15, 0xCD, 0x2D, 0xFA, 0xA7, 0x9D, 0x65,
    0x8F, 0xB1, 0x9B, 0x2F, 0x05, 0xCF, 0x72, 0xA5, 0x59, 0x33, 0x74, 0x0D,
    0x25, 0x49, 0xA4, 0x7B, 0xD9, 0x0D, 0x7F, 0xFE, 0x3F, 0x2C, 0xB5, 0xD1,
    0x1C, 0xEA, 0xB8, 0x08, 0xBA, 0x83, 0x8B, 0xCC, 0x08, 0x1E, 0x76, 0x0F,
    0xF2, 0x47, 0x57, 0x57, 0xEB, 0xEC, 0xB7, 0xF3, 0xC1, 0x30, 0xA2, 0xB6,
    0x53, 0x0A, 0x9F, 0xF9, 0xF8, 0xFC, 0x0F, 0x07, 0x56, 0x9D, 0x8E, 0xDF,
    0x99, 0xB0, 0xE1, 0x67, 0x75, 0xA8, 0x4C, 0x75, 0x0E, 0x80, 0xE1, 0xDF,
    0xE6, 0x9A, 0xB4, 0x22, 0xB9, 0x35, 0x97, 0x35, 0x73, 0xB0, 0xE1, 0xC8,
    0x2D, 0xC5, 0x7A, 0x75, 0x2C, 0x88, 0x59, 0xA8, 0x74, 0x18, 0x29, 0x79,
    0xB9, 0x2E, 0x75, 0xF4, 0x8F, 0xF7, 0x9E, 0x23, 0x8C, 0xE7, 0x21, 0x88,
    0xD1, 0xD0, 0x97, 0x8E, 0xEF, 0xFB, 0x61, 0x50, 0x61, 0x54, 0x6F, 0xD1,
    0xB8, 0xC2, 0x38, 0xE6, 0x99, 0xF7, 0x70, 0x49, 0x21, 0x70, 0xB1, 0x96,
    0xEB, 0xF3, 0x05, 0x65, 0xB6, 0x78, 0x8B, 0x03, 0x68, 0x22, 0xF7, 0x1C,
    0x8E, 0xE3, 0xF7, 0xE7, 0x25, 0xCA, 0xD6, 0xAA, 0x20, 0x02, 0xDB, 0x01,
    0xB8, 0xAE, 0x56, 0xE9, 0x81

};

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

// See 2.4.3.6 PES packet of iso13818-1.pdf
static bool checkFileTSFileH264PESHeaderLength(const uint8_t* transport_stream,
                                               int transport_stream_len) {
  bool success = true;
  const int kTSPacketSize = 188;

  // not a requirement, otherwise need to buffer
  assert((transport_stream_len % kTSPacketSize) == 0);

  for (int offset = 0; offset < transport_stream_len; offset += kTSPacketSize) {
    const uint8_t* p = transport_stream + offset;

    uint16_t pid = ((uint16_t(p[1]) & 0x1f) << 8) | (uint16_t(p[2]));
    uint8_t adaptationFieldControl = (p[3] >> 4) & 0x3;
    if (!(pid == 256        // h264 pid
          && (p[1] & 0x40)  // payload_unit_start_indicator
          && (adaptationFieldControl == 1 || adaptationFieldControl == 3))) {
      continue;
    }

    // if adaptationFieldControl == 1 or 3 there's payload (pes header)
    // if 3 there's adaptation field before payload
    const uint8_t* pesHeader = p + 4;
    if (adaptationFieldControl == 3) {
      // skip over adaptation field to payload
      pesHeader += pesHeader[0] + 1;
    }

    const uint8_t PTS_DTS_flags = pesHeader[7];
    const uint8_t PES_header_data_length = pesHeader[8];

    uint8_t expectedDataLength = 0;

    if ((PTS_DTS_flags & 0xC0) == 0x80) {
      expectedDataLength = 5;  // pts only
    } else if ((PTS_DTS_flags & 0xC0) == 0xC0) {
      expectedDataLength = 10;  // pts and dts
    }

    if (expectedDataLength != PES_header_data_length) {
      success = false;
    }

    // expect h264 start code to follow
    const uint8_t* h264Start = pesHeader + PES_header_data_length + 9;
    if (!(h264Start[0] == 0 && h264Start[1] == 0 && h264Start[2] == 0 &&
          h264Start[3] == 1)) {
      success = false;
    }
  }

  return success;
}

TEST_F(TsMuxerTest, createsCorrectH264PESHeaderLength) {
  ASSERT_TRUE(ts_muxer_ != nullptr);

  for (int i = 0; i < 30; i++) {
    int muxed_len;
    const unsigned char* muxed_data =
        muxH264(ts_muxer_, kH264IntraFrameWithSPSAndPPS, sizeof(kH264IntraFrameWithSPSAndPPS),
                90000 * i / 30, &muxed_len);
    ASSERT_TRUE(checkFileTSFileH264PESHeaderLength(muxed_data, muxed_len));
  }
}

}  // namespace
