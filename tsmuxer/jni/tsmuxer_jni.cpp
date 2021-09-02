#include <jni.h>
#include "tsmuxer/tsmuxer.h"

struct TsMuxer* ts_muxer = nullptr;

// Name of the function matters (will be an execution error if it is not correct).
// In case of error, logs will show you the candidates it searched for
extern "C" JNIEXPORT void JNICALL
Java_com_loom_recorder_RealTsMuxer_initTsMuxer(JNIEnv *env, jobject type) {
    ts_muxer = createTsMuxer();
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_loom_recorder_RealTsMuxer_muxH264(JNIEnv *env, jobject type, jobject byte_buffer_data,
                                           jint position, jint limit,
                                           jlong pts_in_90khz) {

    // Get pointer to raw byte_buffer_data and length
    const uint8_t * raw_media_data = nullptr;
    long raw_media_length = limit - position;

    raw_media_data = (const uint8_t *)env->GetDirectBufferAddress(byte_buffer_data);

    int muxed_len = 0;
    void* muxed_data = (void*)muxH264(ts_muxer, &raw_media_data[position], raw_media_length, pts_in_90khz, &muxed_len );
    jobject jbuffer = env->NewDirectByteBuffer(muxed_data, muxed_len);
    return jbuffer;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_loom_recorder_RealTsMuxer_muxAac(JNIEnv *env, jobject type, jobject byte_buffer_data,
                                          jint position, jint limit,
                                          jlong pts_in_90khz) {

    // Get pointer to raw byte_buffer_data and length
    const uint8_t * raw_media_data = nullptr;
    long raw_media_length = limit - position;

    raw_media_data = (const uint8_t *)env->GetDirectBufferAddress(byte_buffer_data);

    int muxed_len = 0;
    void* muxed_data = (void*)muxAac(ts_muxer, &raw_media_data[position], raw_media_length, pts_in_90khz, &muxed_len );
    jobject jbuffer = env->NewDirectByteBuffer(muxed_data, muxed_len);
    return jbuffer;
}

