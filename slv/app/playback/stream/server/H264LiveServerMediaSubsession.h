#pragma once
#include <liveMedia.hh>
#include "PlaybackPacket.h"
#include "FramedSourceVideo.h"

class H264LiveServerMediaSubsession : public OnDemandServerMediaSubsession {
public:
    static H264LiveServerMediaSubsession* createNew(UsageEnvironment& env, PlaybackPacket* queue) {
        return new H264LiveServerMediaSubsession(env, queue);
    }

protected:
    H264LiveServerMediaSubsession(UsageEnvironment& env, PlaybackPacket* queue)
        : OnDemandServerMediaSubsession(env, True), m_queue(queue) {}

    virtual FramedSource* createNewStreamSource(unsigned, unsigned& estBitrate) override {
        estBitrate = 5000;
        return H264VideoStreamFramer::createNew(envir(),
               FramedSourceVideo::createNew(envir(), m_queue));
    }

    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
                                     unsigned char,
                                     FramedSource*) override {
        return H264VideoRTPSink::createNew(envir(), rtpGroupsock, 96);
    }

private:
    PlaybackPacket* m_queue;
};
