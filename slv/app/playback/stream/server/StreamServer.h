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
    StreamServer();
    ~StreamServer();

    void createServer(void);

private:    
};

#endif // STREAM_SERVER_H
