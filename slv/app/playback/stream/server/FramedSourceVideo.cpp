#include "FramedSourceVideo.h"

void FramedSourceVideo::doGetNextFrame() {
    AVPacket pkt;

    m_pCPacket->pop(&pkt);
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
    // gettimeofday(&fPresentationTime, NULL);
    if (pkt.pts != AV_NOPTS_VALUE)
    {
        int64_t usec = av_rescale_q(pkt.pts, {1, 90000}, {1, 1000000});
        fPresentationTime.tv_sec = usec / 1000000;
        fPresentationTime.tv_usec = usec % 1000000;
    } else {
        gettimeofday(&fPresentationTime, NULL);
    }
    LOGD("send packet: sec:{}, usec:{}", fPresentationTime.tv_sec, fPresentationTime.tv_usec);

    // done frame remove
    av_packet_unref(&pkt);

    // Notify live555 done copy frame data
    FramedSource::afterGetting(this);
}
