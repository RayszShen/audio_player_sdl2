/** 
 * @file: audio_defines.h
 * @brief: define file
 * @author: Ray  (raymm.shen@qq.com)
 * @date: 2024-06-18 14:13:02
 * @lasteditor: Ray  (raymm.shen@qq.com)
 * @lastedittime: 2024-06-19 13:50:34
 * @copyright: Copyright © 2024 by raymm.shen@qq.com, All Rights Reserved.
 */

#include <vector>
#include <SDL2/SDL.h>

#pragma once

extern bool audio_run;

namespace audio_ns
{

#define SAMPLE_RATE 44100
#define NUM_CHANNELS 1
#define SAMPLE_SIZE 2048
#define BUFFER_SIZE 4096

typedef struct AudioDataBuffer
{
    std::vector<Uint8> audioBuffer;
    size_t writePos;
    size_t readPos;

    AudioDataBuffer()
        : audioBuffer(BUFFER_SIZE * 2)
        , writePos(0)
        , readPos(0)
    {

    }
} *AudioDataBufferPtr;

}