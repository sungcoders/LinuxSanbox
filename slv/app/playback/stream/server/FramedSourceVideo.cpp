#include "FramedSourceVideo.h"

void FramedSourceVideo::doGetNextFrame() {
    AVPacket pkt;

    if (!m_queue->pop(pkt)) {
        handleClosure();
        return;
    }

    unsigned size = pkt.size;

    if (size > fMaxSize) {
        fFrameSize = fMaxSize;
        fNumTruncatedBytes = size - fMaxSize;
    } else {
        fFrameSize = size;
    }

    memcpy(fTo, pkt.data, fFrameSize);

    gettimeofday(&fPresentationTime, NULL);

    av_packet_unref(&pkt);

    FramedSource::afterGetting(this);
}
