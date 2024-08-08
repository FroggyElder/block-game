#ifndef _LCD_H_
#define _LCD_H_

#include <stdexcept>
#include <string>
#include <mutex>
#include <thread>
#include <iostream>

namespace lcdC {
    extern "C" {
        #include <stdio.h>
        #include <stdlib.h>
        #include <sys/stat.h>
        #include <sys/types.h>
        #include <unistd.h>
        #include <fcntl.h>
        #include <sys/mman.h>
        #include <string.h>
        #include <sys/ioctl.h>
        #include <linux/fb.h>
        #include <stdbool.h>

        #include "spng_main.h"
        #include "font.h"
    }

}
//struct to refer to a lcd device
class LcdDevice{
private:
    //lock
    mutable std::mutex m_lcdLock;
    //pointer to mmap
    void* m_ptr;
    //buffer
    void* m_buf;
    //path of screen control file
    std::string path;
    //screen size
    int screen_size;
    //screen width
    int w;
    //screen height
    int h;
    //bytes per pixel
    int pixel_size;
    //find descriptor
    int m_fd;
    //var info
    lcdC::fb_var_screeninfo v_info;
    //font file
    lcdC::font* m_font;

public:
    LcdDevice (const char* path);
    ~LcdDevice ();

    lcdC::font* getFont () {return m_font;}
    int getPixelSize () {return pixel_size;}

    void clearScreen (int color);
    void refresh ();
    bool paintMap (void* map,int width,int height,int x0,int y0);
    bool paintPng (const char* path,int x,int y);
    bool paintText (const char* text,int size,int len,int x,int y);
};


#endif //_LCD_H_