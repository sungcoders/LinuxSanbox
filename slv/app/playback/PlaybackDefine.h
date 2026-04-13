
namespace Playbackdef
{
    enum PlaybackState
    {
        PRE_INIT_E = 0,
        INIT_E,
        PLAY_E,
        PAUSE_E,
        STOP_E,
        COMPLETED_E
    };

    enum PlaybackType
    {
        FILE_PLAY = 0,
        STREAM_FILE,
        STREAM_LIVE,
    };
}