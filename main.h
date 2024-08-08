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
    #define LCD0 "/dev/fb0"
    #define TS0 "/dev/input/event0"
    #define LED0 "/dev/Led"
}
#define ARGB_BLACK 0xff000000

void delay(int milliseconds);
void seedInit ();
void fallThread (board::BlockBoard* board);

#endif