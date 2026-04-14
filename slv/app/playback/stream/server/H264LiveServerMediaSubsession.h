#pragma once
#include <liveMedia.hh>
#include "PlaybackPacket.h"
#include "FramedSourceVideo.h"

class H264LiveServerMediaSubsession : public OnDemandServerMediaSubsession
{
public:
    // Static function return object
    static H264LiveServerMediaSubsession* createNew(UsageEnvironment& env, PlaybackPacket* queue)
    {
        // Return object create with env and queue of H264LiveServerMediaSubsession
        return new H264LiveServerMediaSubsession(env, queue);
    }

protected:
    // reference queue to get packet
    // true: VLC A xem → có pipeline riêng
    // true: VLC B xem → có pipeline riêng
    H264LiveServerMediaSubsession(UsageEnvironment& env, PlaybackPacket* queue)
        : OnDemandServerMediaSubsession(env, True)
        , m_queue(queue)
    {
    }

    // Nguồn dữ liệu video
    // H264VideoStreamFramer
    // - tách NAL unit
    // - đảm bảo frame đúng format RTP
    // - thêm timing
    virtual FramedSource* createNewStreamSource(unsigned, unsigned& estBitrate) override
    {
        estBitrate = 5000;
        return H264VideoStreamFramer::createNew(envir(), FramedSourceVideo::createNew(envir(), m_queue));
    }

    // bộ gửi RTP
    // RTPSink = đóng gói data → gửi qua UDP/TCP
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char, FramedSource*) override
    {
        return H264VideoRTPSink::createNew(envir(), rtpGroupsock, 96);
    }

private:
    PlaybackPacket* m_queue;
};
