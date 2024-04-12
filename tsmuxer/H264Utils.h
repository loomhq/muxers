#ifndef H264UTILS_H_HEADER
#define H264UTILS_H_HEADER

#include <stdint.h>

#include <vector>

namespace H264Utils {
enum class NalType { SliceIdrPicture, SequenceParameterSet };

/**
 Returns: true, if any of the nal types is found in the stream
 */
bool hasAnnexBNalOfType(const unsigned char* data, uint32_t length, const std::vector<NalType>& nal_types_to_find);
}  // namespace H264Utils

#endif  // H264UTILS_H_HEADER
