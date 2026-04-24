#pragma once
#include <liveMedia.hh>
#include "PlaybackPacket.h"
#include "FramedSourceVideo.h"

class H264LiveServerMediaSubsession : public OnDemandServerMediaSubsession
{
public:
    // Static function return object
    static H264LiveServerMediaSubsession* createNew(UsageEnvironment& env, std::shared_ptr<PlaybackPacket> packet)
    {
        // Return object create with env and queue packet of H264LiveServerMediaSubsession
        return new H264LiveServerMediaSubsession(env, packet);
    }

protected:
    // reference queue packet to get packet
    // true: VLC A xem → có pipeline riêng
    // true: VLC B xem → có pipeline riêng
    H264LiveServerMediaSubsession(UsageEnvironment& env, std::shared_ptr<PlaybackPacket> packet)
        : OnDemandServerMediaSubsession(env, True)
        , m_pCPacket(packet)
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
        return H264VideoStreamFramer::createNew(envir(), FramedSourceVideo::createNew(envir(), m_pCPacket));
    }

    // bộ gửi RTP
    // RTPSink = đóng gói data → gửi qua UDP/TCP
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char, FramedSource*) override
    {
        return H264VideoRTPSink::createNew(envir(), rtpGroupsock, 96);
    }

private:
    std::shared_ptr<PlaybackPacket> m_pCPacket;
};
