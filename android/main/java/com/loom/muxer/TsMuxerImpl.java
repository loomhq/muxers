/** Copyright © 2020 Loom. All rights reserved. */

package com.loom.muxer;

import java.nio.ByteBuffer;

public class TsMuxerImpl extends Object implements TsMuxer {
    static {
        System.loadLibrary("tsmuxer-jni");
    }

    public TsMuxerImpl() {
        initTsMuxer();
    }

    private native void initTsMuxer();

    // Context is in the cpp bridge
    private native ByteBuffer muxH264(
        ByteBuffer h264Data,
        int position,
        int limit,
        long ptsIn90Khz
    );

    // Context is in the cpp bridge
    private native ByteBuffer muxAac(
        ByteBuffer adtsData,
        int position,
        int limit,
        long ptsIn90Khz 
    );

    private long prevPts = 0;

    public ByteBuffer muxH264(ByteBuffer h264Data, long ptsInUs) {
        long ptsIn90Khz = (ptsInUs * 9 / 100);
        int position = h264Data.position();
        int limit = h264Data.limit();

        return muxH264(h264Data, position, limit, ptsIn90Khz);
    }
    
    public ByteBuffer muxAac(ByteBuffer adtsData, long ptsInUs) {
        Long ptsIn90Khz = (ptsInUs * 9 / 100);
        int position = adtsData.position();
        int limit = adtsData.limit();

        return muxAac(adtsData, position, limit, ptsIn90Khz);
    }
}
