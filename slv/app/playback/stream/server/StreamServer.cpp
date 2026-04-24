#include "StreamServer.h"

StreamServer::StreamServer(std::shared_ptr<PlaybackDemux> demuxer)
: m_pCdemux(demuxer)
, m_pCpacketVideo(nullptr)
{
}

StreamServer::~StreamServer()
{
}

void StreamServer::initServer()
{
}

void StreamServer::createServer()
{
    scheduler = BasicTaskScheduler::createNew();             // event loop engine (socket (RTSP, RTP), timer, callback async - doGetNextFrame())
    env = BasicUsageEnvironment::createNew(*scheduler);   // context toàn cục của Live555 (logging (*env << "log"), error handling, reference tới TaskScheduler)
    rtspServer = RTSPServer::createNew(*env, 8554);             // RTSP server (listen port 8554, nhận request từ: VLC ffplay vaf quản lý session DESCRIBE, SETUP, PLAY...)

    if (!rtspServer)
    {
        *env << "RTSP server failed\n";
        return;
    }

    m_pCdemux->Start();
    m_pCpacketVideo = m_pCdemux->getPacketVideo();

    // rtsp://<ip>:8554/filestream (Tạo một kênh stream tên là filestream để client xem)
    // URL: filestream
    // info (title): Filestream
    // description: FileStream
    ServerMediaSession* sms = ServerMediaSession::createNew(*env, "filestream", "FileStream", "session file stream");

    // add track to server media session
    sms->addSubsession(H264LiveServerMediaSubsession::createNew(*env, m_pCpacketVideo));

    // attach to live555 server
    rtspServer->addServerMediaSession(sms);

    // Tạo ra URL RTSP đầy đủ để client kết nối tới stream
    char* url = rtspServer->rtspURL(sms);
    *env << "Stream URL: " << url << "\n";

    // event loop
    env->taskScheduler().doEventLoop();

    return;
}
