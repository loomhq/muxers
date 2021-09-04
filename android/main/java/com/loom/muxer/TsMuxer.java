package com.loom.muxer;

import java.nio.ByteBuffer;

public interface TsMuxer {
    ByteBuffer muxH264(ByteBuffer h264Data, long ptsInUs);
    ByteBuffer muxAac(ByteBuffer adtsData, long ptsInUs);

    public static TsMuxer createTsMuxer(){
        return new TsMuxerImpl();
    }
}
