#include "StreamServer.h"

StreamServer::StreamServer()
{
}

StreamServer::~StreamServer()
{
}

// extern void demuxThread(PlaybackPacket*, const char*);

void StreamServer::createServer()
{
    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

    // PlaybackPacket queue;

    // chạy demux thread
    // std::thread t(demuxThread, &queue, "test.mp4");

    RTSPServer* rtspServer = RTSPServer::createNew(*env, 8554);
    if (!rtspServer)
    {
        *env << "RTSP server failed\n";
        return;
    }

    ServerMediaSession* sms = ServerMediaSession::createNew(*env, "filestream", "Filestream", "FileStream");

    // sms->addSubsession(H264LiveServerMediaSubsession::createNew(*env, &queue));

    rtspServer->addServerMediaSession(sms);

    char* url = rtspServer->rtspURL(sms);
    *env << "Stream URL: " << url << "\n";

    env->taskScheduler().doEventLoop();

    return;
}
