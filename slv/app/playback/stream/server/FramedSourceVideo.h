#pragma once
#include <liveMedia.hh>
#include "PlaybackPacket.h"

class FramedSourceVideo : public FramedSource {
public:
    static FramedSourceVideo* createNew(UsageEnvironment& env, PlaybackPacket* queue) {
        return new FramedSourceVideo(env, queue);
    }

protected:
    FramedSourceVideo(UsageEnvironment& env, PlaybackPacket* queue)
        : FramedSource(env), m_queue(queue) {}

    virtual void doGetNextFrame() override;

private:
    PlaybackPacket* m_queue;
};
