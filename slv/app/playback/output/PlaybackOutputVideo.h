#ifndef PLAYBACKOUTPUTVIDEO_H
#define PLAYBACKOUTPUTVIDEO_H

#include "PlaybackOutput.h"

class PlaybackOutputVideo : public PlaybackOutput
{
public:
    PlaybackOutputVideo();
    ~PlaybackOutputVideo() = default;

    void Init(std::shared_ptr<PlaybackFrame> frame);
    void Output() override;

private:
    std::shared_ptr<PlaybackFrame> m_pCFrame;
};

#endif // PLAYBACKOUTPUTVIDEO_H
