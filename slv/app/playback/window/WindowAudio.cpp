#include "WindowAudio.h"

constexpr int SAMPLES = 1024;
constexpr int OUT_CHANNEL = 2;
constexpr int OUT_SAMPLE_RATE = 44100;
constexpr int BYTE_PER_SECOND = OUT_SAMPLE_RATE * 4; // 1 second of audio at 44100 Hz, 16-bit stereo

WindowAudio::WindowAudio()
: m_swrCtx(nullptr)
, m_codecCtx(nullptr)
, m_deviceID(-1)
, m_pClock(nullptr)
{
}

WindowAudio::~WindowAudio()
{
    destroyAudio();
}

void WindowAudio::getDeviceIDAudio()
{
    SDL_AudioSpec spec = {};
    spec.freq = OUT_SAMPLE_RATE;
    spec.format = AUDIO_S16SYS;
    spec.channels = OUT_CHANNEL;
    spec.samples = SAMPLES;
    spec.callback = nullptr;

    m_deviceID = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
    if (m_deviceID == 0)
    {
        LOGE("SDL_OpenAudioDevice failed: {}", SDL_GetError());
        return;
    }
    SDL_PauseAudioDevice(m_deviceID, 0);
}

void WindowAudio::audioInit(AVCodecContext* codecCtx, std::shared_ptr<PlaybackClock> clock)
{
    SDL_Init(SDL_INIT_AUDIO);
    m_codecCtx = codecCtx;
    m_pClock = clock;
    getDeviceIDAudio();
    resampleAudio();
    LOGE("Audio device initialized with ID: {}", m_deviceID);
}

void WindowAudio::resampleAudio()
{
    if(m_codecCtx == nullptr)
    {
        LOGE("Codec context is null, cannot initialize resampling");
        return;
    }

    m_swrCtx = swr_alloc_set_opts(
        nullptr,
        AV_CH_LAYOUT_STEREO,
        AV_SAMPLE_FMT_S16,
        OUT_SAMPLE_RATE,
        m_codecCtx->channel_layout,
        m_codecCtx->sample_fmt,
        m_codecCtx->sample_rate,
        0,
        nullptr
    );

    if (!m_swrCtx || swr_init(m_swrCtx) < 0)
    {
        LOGE("m_swrCtx init failed\n");
        return;
    }

    LOGD("Audio resampling initialized successfully");
}

void WindowAudio::sendFrameOutput(const FrameInfo& sframe)
{
    int out_nb_samples = av_rescale_rnd(
        swr_get_delay(m_swrCtx, sframe.frame->sample_rate) + sframe.frame->nb_samples,
        sframe.frame->sample_rate,
        sframe.frame->sample_rate,
        AV_ROUND_UP);

    uint8_t* out_buffer = nullptr;
    int out_linesize = 0;

    av_samples_alloc(&out_buffer,
                        &out_linesize,
                        OUT_CHANNEL,
                        out_nb_samples,
                        AV_SAMPLE_FMT_S16,
                        0);

    int samples = swr_convert(
        m_swrCtx,
        &out_buffer,
        out_nb_samples,
        (const uint8_t**)sframe.frame->data,
        sframe.frame->nb_samples);

    int data_size = av_samples_get_buffer_size(
        &out_linesize,
        OUT_CHANNEL,
        samples,
        AV_SAMPLE_FMT_S16,
        1);

    /*
        sample rate = 44100 samples / giây
        format = S16 (16-bit = 2 bytes)
        channels = 2 (stereo)
        => 1 sample frame = 2 bytes * 2 channels = 4 bytes
        1s = 44100 sample frames * 4 bytes = 44100 * 4 bytes
        thời điểm đang phát thật = thời điểm đã nạp - phần chưa phát
        m_audioCurPts = m_audioPts - queued_time;
    */

    uint32_t queued = SDL_GetQueuedAudioSize(m_deviceID);                               // sample not output in queue
    double duration = (double)sframe.frame->nb_samples / sframe.frame->sample_rate;     // duration frame output
    double m_audioPts = sframe.timestamp + duration;                                    // current time + duration output = time output target
    double queued_time = (double)queued / BYTE_PER_SECOND;                              // current time in queue not output
    double m_audioCurPts = m_audioPts - queued_time;                                    // current time playing = time output target - current time in queue not output
    // LOGW("audio timestamp [duration={:.3f}] {:.3f}s >< {:.3f}s queue:{}", duration, sframe.timestamp, m_audioCurPts, queued);
    m_pClock->setClock(m_audioCurPts, true);

    while (SDL_GetQueuedAudioSize(m_deviceID) > BYTE_PER_SECOND)
    {
        SDL_Delay(10);
    }

    //write audio data to the device
    // LOGE("Queueing {} bytes of audio data", data_size);
    SDL_QueueAudio(m_deviceID, out_buffer, data_size);

    av_free(out_buffer);
}

void WindowAudio::destroyAudio()
{
    if (m_deviceID != -1)
    {
        SDL_CloseAudioDevice(m_deviceID);
        m_deviceID = -1;
    }

    if (m_swrCtx)
    {
        swr_free(&m_swrCtx);
    }

    SDL_Quit();
}
