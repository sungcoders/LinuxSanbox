#pragma once
#include <liveMedia.hh>
#include "PlaybackPacket.h"

class FramedSourceVideo : public FramedSource
{
public:
    static FramedSourceVideo* createNew(UsageEnvironment& env, std::shared_ptr<PlaybackPacket> packet)
    {
        return new FramedSourceVideo(env, packet);
    }

protected:
    FramedSourceVideo(UsageEnvironment& env, std::shared_ptr<PlaybackPacket> packet)
        : FramedSource(env), m_pCPacket(packet)
    {
    }

    // callback get frame
    virtual void doGetNextFrame() override;

private:
    std::shared_ptr<PlaybackPacket> m_pCPacket;
};
