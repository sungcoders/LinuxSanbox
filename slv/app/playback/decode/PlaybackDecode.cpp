#include "PlaybackDecode.h"

PlaybackDecode::PlaybackDecode()
: m_pCPacket(nullptr)
{
    m_pCPacket = std::make_shared<PlaybackPacket>();
}

PlaybackDecode::~PlaybackDecode()
{
    decodeThread.join();
}

void PlaybackDecode::Start()
{
    LOGW("Starting decode thread...");
    decodeThread = std::thread(&PlaybackDecode::Decode, this);
}
