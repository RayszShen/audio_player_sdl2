/** 
 * @file: audio_player.cpp
 * @brief: class <AudioPlayer> source file
 * @author: Ray  (raymm.shen@qq.com)
 * @date: 2024-06-18 14:01:49
 * @lasteditor: Ray  (raymm.shen@qq.com)
 * @lastedittime: 2024-06-19 17:20:15
 * @copyright: Copyright © 2024 by raymm.shen@qq.com, All Rights Reserved.
 */

#include <iostream>
#include "audio/audio_player.h"

namespace audio_ns
{

std::mutex AudioPlayer::m_mutex;
std::condition_variable AudioPlayer::m_cv;
bool AudioPlayer::m_isDataReady = false;

AudioPlayer::AudioPlayer()
    : m_recordDeviceId(0)
    , m_playDeviceId(0)
{
    SDL_zero(m_desiredSpec);
    m_desiredSpec.freq = SAMPLE_RATE;
    m_desiredSpec.format = AUDIO_S16SYS;    // 16-bit signed audio
    m_desiredSpec.channels = NUM_CHANNELS;
    m_desiredSpec.samples = BUFFER_SIZE;
}

AudioPlayer::~AudioPlayer()
{

}

bool AudioPlayer::initPlayer()
{
    if(SDL_Init(SDL_INIT_AUDIO) != 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;

        return false;
    }

    std::cout << "SDL_Init successfully!" << std::endl;

    return true;
}

bool AudioPlayer::startPlayer()
{
    if(startRecordDevice() && startPlayDevice()) 
    {
        // start recording and palyback
        SDL_PauseAudioDevice(m_recordDeviceId, 0);
        SDL_PauseAudioDevice(m_playDeviceId, 0);
        std::cout << "audio player start successfully!" << std::endl;
        std::cout << "working ..." << std::endl;

        return true;
    }
    if(m_recordDeviceId)
    {
        SDL_CloseAudioDevice(m_recordDeviceId);
    }
    if(m_playDeviceId)
    {
        SDL_CloseAudioDevice(m_playDeviceId);
    }

    std::cout << "audio player start failed!" << std::endl;

    return false;
}

void AudioPlayer::stopPlayer()
{
    // stop recording and playback
    SDL_PauseAudioDevice(m_recordDeviceId, 1);
    SDL_PauseAudioDevice(m_playDeviceId, 1);
    SDL_Delay(10);
    if(m_recordDeviceId)
    {
        SDL_CloseAudioDevice(m_recordDeviceId);
    }
    if(m_playDeviceId)
    {
        SDL_CloseAudioDevice(m_playDeviceId);
    }

    SDL_Delay(100);

    SDL_Quit();

    std::cout << "audio player has already exited!" << std::endl;
}

bool AudioPlayer::startRecordDevice()
{
    SDL_AudioSpec recordSpec = m_desiredSpec;
    recordSpec.callback = AudioPlayer::audioRecordingCallback;
    recordSpec.userdata = &m_audioDataBuffer;
    m_recordDeviceId = SDL_OpenAudioDevice(NULL, SDL_TRUE, &recordSpec, &m_obtainedSpec, 0);
    // std::cout << "obtainedSpec.freq: " << m_obtainedSpec.freq 
                //  << " , obtainedSpec.format: " << m_obtainedSpec.format 
                //  << ", obtainedSpec.channels: " << m_obtainedSpec.channels 
                //  << ", obtainedSpec.samples: " << m_obtainedSpec.samples 
                //  << std::endl;
    if(m_recordDeviceId == 0)
    {
        std::cerr << "SDL_OpenAudioDevice (recording) failed: " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }

    return true;
}

bool AudioPlayer::startPlayDevice()
{
    SDL_AudioSpec playSpec = m_desiredSpec;
    playSpec.callback = AudioPlayer::audioPlaybackCallback;
    playSpec.userdata = &m_audioDataBuffer;
    m_playDeviceId = SDL_OpenAudioDevice(NULL, SDL_FALSE, &playSpec, &m_obtainedSpec, 0);
    if(m_playDeviceId == 0)
    {
        std::cerr << "SDL_OpenAudioDevice (playback) failed: " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }

    return true;
}

void AudioPlayer::audioRecordingCallback(void* userdata, Uint8* stream, int len)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    AudioDataBufferPtr recordDataBufferPtr = static_cast<AudioDataBufferPtr>(userdata);
    size_t spaceLeft = recordDataBufferPtr->audioBuffer.size() - recordDataBufferPtr->writePos;
    if(spaceLeft < len)
    {
        SDL_memcpy(&recordDataBufferPtr->audioBuffer[recordDataBufferPtr->writePos], stream, spaceLeft);
        SDL_memcpy(&recordDataBufferPtr->audioBuffer[0], stream + spaceLeft, len - spaceLeft);
    }
    else
    {
        SDL_memcpy(&recordDataBufferPtr->audioBuffer[recordDataBufferPtr->writePos], stream, len);
    }
    recordDataBufferPtr->writePos = (recordDataBufferPtr->writePos + len) % recordDataBufferPtr->audioBuffer.size();
    m_isDataReady = true;
    m_cv.notify_one();
}

void AudioPlayer::audioPlaybackCallback(void* userdata, Uint8* stream, int len)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, []() { return m_isDataReady; });
    AudioDataBufferPtr playDataBufferPtr = static_cast<AudioDataBufferPtr>(userdata);
    size_t spaceLeft = playDataBufferPtr->audioBuffer.size() - playDataBufferPtr->readPos;
    SDL_memset(stream, 0, len);
    if(spaceLeft < len)
    {
        SDL_memcpy(stream, &playDataBufferPtr->audioBuffer[playDataBufferPtr->readPos], spaceLeft);
        SDL_memcpy(stream + spaceLeft, &playDataBufferPtr->audioBuffer[0], len - spaceLeft);
    }
    else
    {
        SDL_memcpy(stream, &playDataBufferPtr->audioBuffer[playDataBufferPtr->readPos], len);
    }
    playDataBufferPtr->readPos = (playDataBufferPtr->readPos + len) % playDataBufferPtr->audioBuffer.size();
    if(playDataBufferPtr->readPos == playDataBufferPtr->writePos)
    {
        m_isDataReady = false;
    }
}

}