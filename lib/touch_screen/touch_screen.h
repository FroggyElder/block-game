#ifndef _TOUCH_SCREEN_H_
#define _TOUCH_SCREEN_H_

#include <stdexcept>
#include <iostream>
#include <mutex>

extern "C" {
    namespace Tsc {
        typedef unsigned short __u16;
        typedef int __s32;
        typedef unsigned char __u8;
        typedef unsigned int __u32;
        typedef short __s16;
        #include <stdio.h>
        #include <stdlib.h>
        #include <sys/stat.h>
        #include <sys/types.h>
        #include <unistd.h>
        #include <fcntl.h>
        #include <linux/input.h>
        #include <sys/mman.h>
        #include <stdbool.h>
    }
}
#define X_TOUCH_TO_LCD 800/1024
#define Y_TOUCH_TO_LCD 480/600

struct tscreen_status {
    bool pressed;
    int x;
    int y;
};

class Tscreen {
private:
    std::mutex m_lock{};
    int fd;
    tscreen_status status;

public:
    Tscreen(const char* path);
    ~Tscreen();

    tscreen_status& getStatus() 
    {
        std::lock_guard<std::mutex> lock(m_lock);
        return status;
    }
    int updateStatus ();

    enum class Action {
        tap,up,down,left,right
    };
    Tscreen::Action getAction ();
};

inline std::ostream& operator<<(std::ostream& os, const Tscreen::Action& action) {
    switch(action) {
        case Tscreen::Action::tap:
            os << "tap";
            break;
        case Tscreen::Action::up:
            os << "up";
            break;
        case Tscreen::Action::down:
            os << "down";
            break;
        case Tscreen::Action::left:
            os << "left";
            break;
        case Tscreen::Action::right:
            os << "right";
            break;
    }
    return os;
}

#endif //_TOUCH_SCREEN__H_