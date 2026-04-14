#include "FramedSourceVideo.h"

void FramedSourceVideo::doGetNextFrame() {
    AVPacket pkt;

    m_queue->pop(&pkt);
    // Notify live555 out of data stream
    // handleClosure();
    // return;

    unsigned size = pkt.size;

    // only copy data with fMaxSize
    if (size > fMaxSize)
    {
        fFrameSize = fMaxSize;
        fNumTruncatedBytes = size - fMaxSize;
    }
    else
    {
        fFrameSize = size;
    }

    // copy data to buffer live555
    memcpy(fTo, pkt.data, fFrameSize);

    // get current time
    gettimeofday(&fPresentationTime, NULL);

    // done frame remove
    av_packet_unref(&pkt);

    // Notify live555 done copy frame data
    FramedSource::afterGetting(this);
}
