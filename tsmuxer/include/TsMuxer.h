#ifndef TSMUXER_H_HEADER
#define TSMUXER_H_HEADER

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

const uint32_t TSMUXER_HAS_H264   = 1 << 0;
const uint32_t TSMUXER_HAS_AAC    = 1 << 1;

struct TsMuxer;
const unsigned char* muxAac(struct TsMuxer* muxer,
                            const unsigned char* data,
                            int length,
                            unsigned long pts_in_90khz,
                            int* muxed_len);

const unsigned char* muxH264(struct TsMuxer* muxer,
                             const unsigned char* data,
                             int length,
                             unsigned long pts_in_90khz,
                             int* muxed_len);

// Pass in a combination of flags defined above.
// e.g. createTsMuxer(HAS_H264 | HAS_AAC)
struct TsMuxer* createTsMuxer(uint32_t flags);

void destroyTsMuxer(struct TsMuxer* ts_muxer);

#ifdef __cplusplus
}
#endif

#endif  // TSMUXER_H_HEADER
