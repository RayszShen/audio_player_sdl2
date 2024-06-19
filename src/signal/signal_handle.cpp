/** 
 * @file: signal_handle.cpp
 * @brief: signal_handle source file
 * @author: Ray  (raymm.shen@qq.com)
 * @date: 2024-06-19 13:34:56
 * @lasteditor: Ray  (raymm.shen@qq.com)
 * @lastedittime: 2024-06-19 13:47:41
 * @copyright: Copyright © 2024 by raymm.shen@qq.com, All Rights Reserved.
 */

#include <iomanip>
#include <iostream>
#include "signal/signal_handle.h"

bool audio_run = true;

void initSignalHandle()
{
   signal(SIGINT, signalHandle);
   signal(SIGILL, signalHandle);
   signal(SIGABRT, signalHandle);
   signal(SIGFPE, signalHandle);
   signal(SIGSEGV, signalHandle);
   signal(SIGTERM, signalHandle);

   #if defined(__linux__) || defined(__unix__)
      signal(SIGUSR2, signalHandle);
      signal(SIGALRM, signalHandle);
      signal(SIGKILL, signalHandle);
      signal(SIGUSR1, signalHandle);
      signal(SIGBUS, signalHandle);
      signal(SIGTRAP, signalHandle);
      signal(SIGQUIT, signalHandle);
      signal(SIGPIPE, signalHandle);
      signal(SIGSTKFLT, signalHandle);
      signal(SIGCLD, signalHandle);
      signal(SIGCHLD, signalHandle);
      signal(SIGCONT, signalHandle);
      signal(SIGSTOP, signalHandle);
      signal(SIGTSTP, signalHandle);
      signal(SIGTTIN, signalHandle);
      signal(SIGURG, signalHandle);
      signal(SIGXCPU, signalHandle);
      signal(SIGXFSZ, signalHandle);
      signal(SIGIO, SIG_IGN);
      signal(SIGWINCH, SIG_IGN);
      signal(SIGPROF, SIG_IGN);
   #endif
}

void signalHandle(int sigNum)
{
   #if defined(__linux__) || defined(__unix__)
      if (sigNum == SIGPIPE)
      {
         std::cout << "signal pipe raised, now ignore !!!" << std::endl;
      } 
      else if (sigNum == SIGCHLD)
      {
         std::cout << "signal SIGCHLD, now ignore !!!" << std::endl;
      }
   #endif

   if (sigNum == SIGTERM)
   {
      std::cout << "signal SIGTERM, now ignore !!!" << std::endl;
   }
   else if (sigNum == SIGINT)
   {
      std::cout << "signal ctrl c, now ignore !!!" << std::endl;
      audio_run = false;
   }
   else
   {

   }
}