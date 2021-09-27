
#include <tsmuxer/H264Utils.h>

#include <gtest/gtest.h>

namespace {

// Contains SPS, PPS, SEI, IDR
const uint8_t kVideoToolsH264Frame1[] = {
    0x00, 0x00, 0x00, 0x01, 0x27, 0x4d, 0x40, 0x0b, 0xa9, 0x18, 0x40, 0xaf,
    0x2f, 0x80, 0xb7, 0x02, 0x02, 0x02, 0x30, 0xad, 0x7b, 0xdf, 0x01, 0x00,
    0x00, 0x00, 0x01, 0x28, 0xfe, 0x09, 0x88, 0x00, 0x00, 0x00, 0x01, 0x06,
    0x00, 0x07, 0x81, 0xc9, 0xc3, 0x80, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00,
    0x00, 0x01, 0x06, 0x05, 0x11, 0x03, 0x87, 0xf4, 0x4e, 0xcd, 0x0a, 0x4b,
    0xdc, 0xa1, 0x94, 0x3a, 0xc3, 0xd4, 0x9b, 0x17, 0x1f, 0x00, 0x80, 0x00,
    0x00, 0x00, 0x01, 0x25, 0xb8, 0x20, 0x20, 0xbf, 0xeb, 0x08, 0x6a, 0x0c,
    0xff, 0x03, 0xce, 0x9b, 0xc0, 0x7d, 0xa8, 0xab, 0x83, 0x89, 0x47, 0xc5,
    0x3a, 0xe5, 0x01, 0x95, 0xdf, 0x7e, 0x23, 0xa3, 0x61, 0x2b, 0xc9, 0xf2,
    0xfc, 0x86, 0xad, 0x6f, 0xba, 0x74, 0xdb, 0x3c, 0x4e, 0xc4, 0x0c, 0x5c,
    0x99, 0xfe, 0x54, 0xf6, 0xce, 0x4c, 0x42, 0x6a, 0x28, 0x2c, 0x00, 0x33,
    0x78, 0x20, 0x18, 0x00, 0x00, 0x00, 0x01, 0x25, 0x05, 0x2e, 0x08, 0x08,
    0x2f, 0xeb, 0x08, 0x6a, 0x0c, 0xff, 0x03, 0xce, 0x9b, 0xc0, 0x7d, 0xa8,
    0xab, 0x83, 0x89, 0x47, 0xc5, 0x3a, 0xe5, 0x01, 0x95, 0xdf, 0x7e, 0x23,
    0xa3, 0x61, 0x2b, 0xc9, 0xf2, 0xfc, 0x86, 0xad, 0x6f, 0xba, 0x74, 0xdb,
    0x3c, 0x4e, 0xc4, 0x0c, 0x5c, 0x99, 0xfe, 0x54, 0xf6, 0xce, 0x4c, 0x42,
    0x6a, 0x28, 0x2c, 0x00, 0x33, 0x78, 0x20, 0x18};

// Contains SEI + non IDR
const uint8_t kVideoToolsH264Frame2[] = {
    0x00, 0x00, 0x00, 0x01, 0x06, 0x05, 0x11, 0x03, 0x87, 0xf4,
    0x4e, 0xcd, 0x0a, 0x4b, 0xdc, 0xa1, 0x94, 0x3a, 0xc3, 0xd4,
    0x9b, 0x17, 0x1f, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0x21,
    0xe1, 0x05, 0x13, 0xff, 0x00, 0x13, 0x10, 0x00, 0x00, 0x00,
    0x01, 0x21, 0x05, 0x38, 0x41, 0x44, 0xff, 0x00, 0x13, 0x10};

const uint8_t kMediaFoundationFrame1[] = {
    0x00, 0x00, 0x00, 0x01, 0x09, 0x10, 0x00, 0x00, 0x00, 0x01, 0x67, 0x42,
    0xc0, 0x1e, 0x95, 0xa1, 0x49, 0xfb, 0xc0, 0x44, 0x00, 0x00, 0x03, 0x00,
    0x04, 0x00, 0x00, 0x03, 0x00, 0xf0, 0x36, 0x82, 0x21, 0x1a, 0x80, 0x00,
    0x00, 0x00, 0x01, 0x68, 0xce, 0x38, 0x80, 0x00, 0x00, 0x00, 0x01, 0x06,
    0x05, 0x2f, 0x02, 0xf8, 0x61, 0x50, 0xfc, 0x70, 0x41, 0x72, 0xb7, 0x32,
    0x48, 0xf3, 0xa7, 0x2a, 0x3d, 0x34, 0x4d, 0x69, 0x63, 0x72, 0x6f, 0x73,
    0x6f, 0x66, 0x74, 0x20, 0x48, 0x2e, 0x32, 0x36, 0x34, 0x20, 0x45, 0x6e,
    0x63, 0x6f, 0x64, 0x65, 0x72, 0x20, 0x56, 0x31, 0x2e, 0x35, 0x2e, 0x33,
    0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0x06, 0x05, 0xe9, 0xcb, 0xb2, 0x13,
    0x92, 0x98, 0x73, 0x43, 0xda, 0xa8, 0xa6, 0xc7, 0x42, 0x98, 0x35, 0x6c,
    0xf5, 0x73, 0x72, 0x63, 0x3a, 0x33, 0x20, 0x68, 0x3a, 0x36, 0x30, 0x20,
    0x77, 0x3a, 0x38, 0x30, 0x20, 0x66, 0x70, 0x73, 0x3a, 0x33, 0x30, 0x2e,
    0x30, 0x30, 0x30, 0x20, 0x70, 0x66, 0x3a, 0x36, 0x36, 0x20, 0x6c, 0x76,
    0x6c, 0x3a, 0x38, 0x20, 0x62, 0x3a, 0x30, 0x20, 0x62, 0x71, 0x70, 0x3a,
    0x33, 0x20, 0x67, 0x6f, 0x70, 0x3a, 0x39, 0x30, 0x20, 0x69, 0x64, 0x72,
    0x3a, 0x39, 0x30, 0x20, 0x73, 0x6c, 0x63, 0x3a, 0x31, 0x20, 0x63, 0x6d,
    0x70, 0x3a, 0x30, 0x20, 0x72, 0x63, 0x3a, 0x31, 0x20, 0x71, 0x70, 0x3a,
    0x32, 0x36, 0x20, 0x72, 0x61, 0x74, 0x65, 0x3a, 0x35, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x20, 0x70, 0x65, 0x61, 0x6b, 0x3a, 0x30, 0x20, 0x62,
    0x75, 0x66, 0x66, 0x3a, 0x31, 0x38, 0x37, 0x35, 0x30, 0x30, 0x30, 0x20,
    0x72, 0x65, 0x66, 0x3a, 0x31, 0x20, 0x73, 0x72, 0x63, 0x68, 0x3a, 0x33,
    0x32, 0x20, 0x61, 0x73, 0x72, 0x63, 0x68, 0x3a, 0x31, 0x20, 0x73, 0x75,
    0x62, 0x70, 0x3a, 0x31, 0x20, 0x70, 0x61, 0x72, 0x3a, 0x36, 0x20, 0x33,
    0x20, 0x33, 0x20, 0x72, 0x6e, 0x64, 0x3a, 0x30, 0x20, 0x63, 0x61, 0x62,
    0x61, 0x63, 0x3a, 0x30, 0x20, 0x6c, 0x70, 0x3a, 0x30, 0x20, 0x63, 0x74,
    0x6e, 0x74, 0x3a, 0x30, 0x20, 0x61, 0x75, 0x64, 0x3a, 0x31, 0x20, 0x6c,
    0x61, 0x74, 0x3a, 0x31, 0x20, 0x77, 0x72, 0x6b, 0x3a, 0x31, 0x20, 0x76,
    0x75, 0x69, 0x3a, 0x31, 0x20, 0x6c, 0x79, 0x72, 0x3a, 0x31, 0x20, 0x3c,
    0x3c, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0x65, 0x88, 0x80, 0x4f, 0xfe,
    0x3f, 0xc3, 0x71, 0x40, 0x00, 0x40, 0x2f, 0xc0, 0x70, 0x98, 0x44, 0x4b,
    0x6f, 0x00, 0x00, 0x81, 0xf4, 0x12, 0x1c, 0x00, 0x04, 0x0b, 0x23, 0x80,
    0x00, 0x81, 0x64, 0x01, 0x83, 0x55, 0x47, 0x60, 0xc3, 0xfb, 0xcf, 0x8f,
    0xff, 0x88, 0xff, 0x85, 0x38, 0x01, 0x2c, 0xce, 0x22, 0x1a, 0xa2, 0xed,
    0x70, 0x1f, 0x80, 0x19, 0x4f, 0x93, 0x69, 0x8f, 0xbf, 0xff, 0x7f, 0xf2,
    0x7f, 0xff, 0xe0, 0x92, 0x28, 0x00, 0x08, 0x1f, 0xf8, 0x80, 0x00, 0x20,
    0x64, 0x00, 0x02, 0x02, 0xe8, 0x40, 0x00, 0x10, 0x32, 0x00, 0x01, 0x01,
    0x74, 0xd4, 0xb8, 0xd6, 0x3d, 0x7f, 0xff, 0x80, 0x06, 0xd3, 0xfd, 0x2e,
    0x9e, 0xfb, 0xc2, 0xeb, 0xfe, 0x8f, 0xfd, 0x7e, 0x00, 0xc0, 0xf5, 0x79,
    0xdb, 0xfe, 0xf2, 0xe5, 0xe3, 0x84, 0xb8, 0x01, 0x83, 0x55, 0x40, 0x00,
    0x20, 0x68, 0x76, 0xef, 0x4a, 0xfb, 0xe1, 0x7d, 0x03, 0xff, 0x02, 0x61,
    0xe4, 0x36, 0xa7, 0xff, 0xf3, 0x58, 0x00, 0x69, 0xaf, 0x93, 0xf9, 0xcf,
    0xad, 0x5e, 0xb6, 0xdc, 0x04, 0x2d, 0xd5, 0xeb, 0x63, 0xdf, 0x4a, 0x7f,
    0xff, 0xe0, 0x0c, 0x0f, 0x57, 0x9d, 0xb1, 0xff, 0xfe, 0x0a, 0xe2, 0x80,
    0x00, 0x81, 0x9f, 0x8e, 0x00, 0x02, 0x10, 0xd1, 0xc0, 0x00, 0x42, 0x1a,
    0x00, 0xc1, 0xaa, 0xa3, 0xb7, 0x87, 0xf6, 0xa2, 0xb6, 0xb1, 0xea, 0x02,
    0x4f, 0xf0, 0x1f, 0x4c, 0xbf, 0x00, 0x0d, 0xa7, 0xfa, 0x5d, 0x3d, 0xf7,
    0xff, 0x81, 0x30, 0xf2, 0x1b, 0x53, 0xfb, 0x50, 0xe3, 0x2d};

TEST(H264UtilsTest, shouldNotCrash_withTooShortNals) {
  ASSERT_FALSE(H264Utils::hasAnnexBNalOfType(
      nullptr, 0, {H264Utils::NalType::SliceIdrPicture}));

  std::vector<uint8_t> data;
  data = {0x00};
  ASSERT_FALSE(H264Utils::hasAnnexBNalOfType(
      data.data(), data.size(), {H264Utils::NalType::SliceIdrPicture}));

  data = {0x00, 0x00};
  ASSERT_FALSE(H264Utils::hasAnnexBNalOfType(
      data.data(), data.size(), {H264Utils::NalType::SliceIdrPicture}));

  data = {0x00, 0x00, 0x00};
  ASSERT_FALSE(H264Utils::hasAnnexBNalOfType(
      data.data(), data.size(), {H264Utils::NalType::SliceIdrPicture}));
}

TEST(H264UtilsTest, shouldNotCrash_withMaliciousData) {
  std::vector<uint8_t> data;

  // 3 bytes NAL start code (and nothing else)
  data = {0x00, 0x00, 0x01};
  ASSERT_FALSE(H264Utils::hasAnnexBNalOfType(
      data.data(), data.size(), {H264Utils::NalType::SliceIdrPicture}));

  // 4 bytes NAL start code (and nothing else)
  data = {0x00, 0x00, 0x00, 0x01};
  ASSERT_FALSE(H264Utils::hasAnnexBNalOfType(
      data.data(), data.size(), {H264Utils::NalType::SliceIdrPicture}));

  // Zeros + 4 byte start code (and nothing else)
  data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
  ASSERT_FALSE(H264Utils::hasAnnexBNalOfType(
      data.data(), data.size(), {H264Utils::NalType::SliceIdrPicture}));
}

TEST(H264UtilsTest, shouldFindNal_withLongHeadingZeros) {
  std::vector<uint8_t> data;

  data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x05};
  ASSERT_TRUE(H264Utils::hasAnnexBNalOfType(
      data.data(), data.size(), {H264Utils::NalType::SliceIdrPicture}));
}

TEST(H264UtilsTest, shouldFindNal_withLongZerosIntheMiddle) {
  std::vector<uint8_t> data;

  data = {0x00, 0x00, 0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x05};
  ASSERT_TRUE(H264Utils::hasAnnexBNalOfType(
      data.data(), data.size(), {H264Utils::NalType::SliceIdrPicture}));
}

TEST(H264UtilsTest, returnsTrue_IfOneNalIsFound) {
  std::vector<uint8_t> data;

  data = {0x00, 0x00, 0x01, 0x06, 0x00, 0x00, 0x01, 0x05};
  ASSERT_TRUE(H264Utils::hasAnnexBNalOfType(
      data.data(), data.size(), {H264Utils::NalType::SliceIdrPicture}));
}

TEST(H264UtilsTest, returnsFalse_IfNoNalIsFound) {
  std::vector<uint8_t> data;

  data = {0x00, 0x00, 0x01, 0x06, 0x00, 0x00, 0x01, 0x04};
  ASSERT_FALSE(H264Utils::hasAnnexBNalOfType(
      data.data(), data.size(), {H264Utils::NalType::SliceIdrPicture}));
}

TEST(H264UtilsTest, shouldFindNal_inRealMacH264Frame) {
  ASSERT_TRUE(H264Utils::hasAnnexBNalOfType(
      kVideoToolsH264Frame1, sizeof(kVideoToolsH264Frame1),
      {H264Utils::NalType::SequenceParameterSet}));
  ASSERT_TRUE(H264Utils::hasAnnexBNalOfType(
      kVideoToolsH264Frame1, sizeof(kVideoToolsH264Frame1),
      {H264Utils::NalType::SliceIdrPicture}));
}

TEST(H264UtilsTest, shouldNotFindIntra_inRealMacInterFrame) {
  ASSERT_FALSE(H264Utils::hasAnnexBNalOfType(
      kVideoToolsH264Frame2, sizeof(kVideoToolsH264Frame2),
      {H264Utils::NalType::SliceIdrPicture,
       H264Utils::NalType::SequenceParameterSet}));
}

TEST(H264UtilsTest, shouldFindNal_inRealWindowsH264Frame) {
  ASSERT_TRUE(H264Utils::hasAnnexBNalOfType(
      kMediaFoundationFrame1, sizeof(kMediaFoundationFrame1),
      {H264Utils::NalType::SequenceParameterSet}));
  ASSERT_TRUE(H264Utils::hasAnnexBNalOfType(
      kMediaFoundationFrame1, sizeof(kMediaFoundationFrame1),
      {H264Utils::NalType::SliceIdrPicture}));
}

}  // namespace
