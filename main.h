#ifndef _MAIN_H__
#define _MAIN_H__

#include <string>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <utility>

#include "lcd.h"
#include "touch_screen.h"
#include "screenObj.h"
#include "block.h"
#include "menu.h"


namespace dev
{
    ///path to ur lcd device
    #define LCD0 "/dev/fb0"
    //path to ur touchScreen
    #define TS0 "/dev/input/event0"
    //path to ur led
    #define LED0 "/dev/Led"
}
#define ARGB_BLACK 0xff000000

//simple function to delay for milliseconds
void delay(int milliseconds);

//initiate random seed with millisecond time
void seedInit ();

//thread to handle fall and delay of object
void fallThread (board::BlockBoard* board);

//terminate the whole program (through pauseMenu)
void fullexit (void* pauseMenu);

//unlock the mutex lock
void mutexUnlock (void* mutex);

//reset board
void boardRestart (void* board);

#endif