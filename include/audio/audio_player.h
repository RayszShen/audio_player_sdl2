/** 
 * @file: audio_player.h
 * @brief: class <AudioPlayer> header file
 * @author: Ray  (raymm.shen@qq.com)
 * @date: 2024-06-18 14:03:06
 * @lasteditor: Ray  (raymm.shen@qq.com)
 * @lastedittime: 2024-06-19 13:31:51
 * @copyright: Copyright © 2024 by raymm.shen@qq.com, All Rights Reserved.
 */

#pragma once

#include <mutex>
#include <condition_variable>
#include "audio_defines.h"

namespace audio_ns
{

class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();

    bool initPlayer();
    bool startPlayer();
    void stopPlayer();

private:
    bool startRecordDevice();
    bool startPlayDevice();

    // record device callback
    static void audioRecordingCallback(void* userdata, Uint8* stream, int len);
    // play device callback
    static void audioPlaybackCallback(void* userdata, Uint8* stream, int len);

private:
    SDL_AudioSpec m_desiredSpec;
    SDL_AudioSpec m_obtainedSpec;

    // shared audio data buffer
    AudioDataBuffer m_audioDataBuffer;

    SDL_AudioDeviceID m_recordDeviceId;
    SDL_AudioDeviceID m_playDeviceId;

    static std::mutex m_mutex;
    static std::condition_variable m_cv;
    static bool m_isDataReady;
};

}