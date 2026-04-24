#ifndef STREAM_SERVER_H
#define STREAM_SERVER_H

#include <thread>
#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include "H264LiveServerMediaSubsession.h"
#include "PlaybackDemux.h"
#include "PlaybackPacket.h"

class StreamServer
{
public:
    StreamServer(std::shared_ptr<PlaybackDemux> demuxer);
    ~StreamServer();

    void initServer();
    void createServer(void);

private:
    std::shared_ptr<PlaybackDemux> m_pCdemux;
    std::shared_ptr<PlaybackPacket> m_pCpacketVideo;
    TaskScheduler* scheduler;
    UsageEnvironment* env;
    RTSPServer* rtspServer;
};

#endif // STREAM_SERVER_H
