/** 
 * @file: main.cpp
 * @brief: main file
 * @author: Ray  (raymm.shen@qq.com)
 * @date: 2024-06-18 15:06:43
 * @lasteditor: Ray  (raymm.shen@qq.com)
 * @lastedittime: 2024-06-19 17:21:09
 * @copyright: Copyright © 2024 by raymm.shen@qq.com, All Rights Reserved.
 */

#include "audio/audio_player.h"
#include "signal/signal_handle.h"

int main()
{
    initSignalHandle();

    audio_ns::AudioPlayer audio;

    if(audio.initPlayer())
    {
        if(audio.startPlayer())
        {
            while(audio_run)
            {
                SDL_Delay(1000);
            }

            audio.stopPlayer();
        }
    }

    return 0;
}