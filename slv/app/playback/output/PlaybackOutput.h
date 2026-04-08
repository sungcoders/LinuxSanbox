#ifndef PLAYBACKOUTPUT_H
#define PLAYBACKOUTPUT_H

#include "PlaybackPacket.h"
#include <unistd.h>
#include "PlaybackWindow.h"

class PlaybackOutput
{
public:
    PlaybackOutput();
    ~PlaybackOutput();

    void Start();
    virtual void Output() = 0;

protected:
    PlaybackWindow win;
    std::shared_ptr<PlaybackFrame> m_pCFrame = std::make_shared<PlaybackFrame>();
    std::thread outputThread;

};

#endif // PLAYBACKOUTPUT_H
