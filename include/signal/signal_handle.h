/** 
 * @file: signal_handle.h
 * @brief: signal_handle header file
 * @author: Ray  (raymm.shen@qq.com)
 * @date: 2024-06-19 13:34:45
 * @lasteditor: Ray  (raymm.shen@qq.com)
 * @lastedittime: 2024-06-19 13:48:10
 * @copyright: Copyright © 2024 by raymm.shen@qq.com, All Rights Reserved.
 */

#pragma once

#include <signal.h>
#include <csignal>
#include "audio/audio_defines.h"

void initSignalHandle();
void signalHandle(int sigNum);