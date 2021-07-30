#ifndef TSMUXER_H_HEADER
#define TSMUXER_H_HEADER

#ifdef __cplusplus
extern "C" {
#endif

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

struct TsMuxer* createTsMuxer();

void destroyTsMuxer(struct TsMuxer* ts_muxer);

#ifdef __cplusplus
}
#endif

#endif  // TSMUXER_H_HEADER
