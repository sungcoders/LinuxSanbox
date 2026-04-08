#ifndef PLAYBACKDECODE_H
#define PLAYBACKDECODE_H

#include "PlaybackFrame.h"
#include "PlaybackPacket.h"
#include <unistd.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

class PlaybackDecode
{
public:
    PlaybackDecode();
    ~PlaybackDecode();

    void Start();
    virtual void Decode() = 0;

protected:
    std::shared_ptr<PlaybackPacket> m_pCPacket;
    std::thread decodeThread;
};

#endif // PLAYBACKDECODE_H
