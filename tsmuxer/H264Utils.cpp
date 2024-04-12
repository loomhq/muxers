#include "H264Utils.h"

#include <assert.h>

#include <algorithm>

uint8_t getNalUniTypeValue(H264Utils::NalType nal_type) {
  switch (nal_type) {
    case H264Utils::NalType::SliceIdrPicture:
      return 5;
    case H264Utils::NalType::SequenceParameterSet:
      return 7;
    default:
      assert(false && "nal unit not encoded");
  }
  return 0;
}

bool H264Utils::hasAnnexBNalOfType(const unsigned char* data, uint32_t length,
                                   const std::vector<NalType>& nal_types_to_find) {
  uint32_t offset = 0;

  if (nal_types_to_find.empty()) {
    return false;
  }

  std::vector<uint8_t> nal_types_to_find_value;
  for (auto nal_type : nal_types_to_find) {
    nal_types_to_find_value.push_back(getNalUniTypeValue(nal_type));
  }

  while (offset < length) {
    bool nal_prefix_found = false;
    uint32_t nal_prefix_length = 0;

    // 4 byte length NAL prefix: 00, 00, 00, 01
    if (offset + 4 < length && data[offset] == 0x00 && data[offset + 1] == 0x00 && data[offset + 2] == 0x00 &&
        data[offset + 3] == 0x01) {
      nal_prefix_found = true;
      nal_prefix_length = 4;
    } else if (offset + 3 < length && data[offset] == 0x00 && data[offset + 1] == 0x00 && data[offset + 2] == 0x01) {
      // 3 byte length NAL prefix: 00, 00, 01
      nal_prefix_found = true;
      nal_prefix_length = 3;
    }

    if (nal_prefix_found) {
      offset += nal_prefix_length;
      uint8_t nal_type = data[offset] & 0x1F;
      if (std::find(nal_types_to_find_value.begin(), nal_types_to_find_value.end(), nal_type) !=
          nal_types_to_find_value.end()) {
        return true;
      }
    }

    offset++;  // Keep searching
  }

  return false;
}
