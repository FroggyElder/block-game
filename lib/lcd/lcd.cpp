#include "lcd.h"
using namespace lcdC;

LcdDevice::LcdDevice (const char* path) {
    //access screen
    m_fd = open(path,O_RDWR|O_NONBLOCK);
    if (m_fd==-1) {
        throw std::runtime_error("screen not available");
    }

    //font
    m_font = fontLoad((char*)"./assets/simfang.ttf");
    
    //get screen info
    if(ioctl(m_fd,FBIOGET_VSCREENINFO,&(this->v_info))==-1) {
        close(m_fd);
        throw std::runtime_error("failed to get screen info");
    }
    
    //put said info into struct
    this->w = this->v_info.xres;
    this->h = this->v_info.yres;
    this->pixel_size = this->v_info.bits_per_pixel/8;
    this->screen_size = this->w*this->h;

    //map the pointer and get the path to screen file
    this->m_ptr = mmap(NULL,this->v_info.xres_virtual * this->v_info.yres_virtual * this->pixel_size,PROT_READ|PROT_WRITE,MAP_SHARED,m_fd,0);
    this->m_buf = this->m_ptr + this->screen_size * this->pixel_size;
    this->path = path;

    //if all is good
    return;
}

LcdDevice::~LcdDevice () {
    close(m_fd);
    fontUnload(m_font);
    munmap((void*)(this->path.c_str()),this->screen_size*4);
}

void LcdDevice::refresh ()
{
    std::lock_guard<std::mutex> lock(m_lcdLock);

    this->v_info.yoffset = this->h;
    ioctl(m_fd,FBIOPAN_DISPLAY, &(this->v_info));
    memcpy (m_ptr,m_buf,this->screen_size*this->pixel_size);
    this->v_info.yoffset = 0;
    ioctl(m_fd,FBIOPAN_DISPLAY, &(this->v_info));
}

void LcdDevice::clearScreen (int color) {

    std::lock_guard<std::mutex> lock(m_lcdLock);

    for (int i=0;i<this->screen_size;i++)
        memcpy(this->m_buf+i*this->pixel_size,&color,this->pixel_size);

}

bool LcdDevice::paintMap (void* map,int width,int height,int x0,int y0) {
    //adjust the paint size if about to be out of range
    if (x0+width>this->w) width=this->w-x0;
    if (y0+height>this->h) height=this->h-y0;

    std::lock_guard<std::mutex> lock(m_lcdLock);

    //now paint it
    for (int i=0;i<height;i++)
        memcpy(m_buf+this->pixel_size*(this->w*(i+y0)+x0),map+width*i*this->pixel_size,width*this->pixel_size);

    //if all is good
    return true;
}

bool LcdDevice::paintPng (const char* path,int x,int y)
{
    unsigned char* png_matrix;
    size_t height,width;

    lcdC::read_png_to_argb(path,&png_matrix,&width,&height);

    for (int i=0;i<(width*height);i++)
    {
        unsigned int* p = (unsigned int*)png_matrix + i;
        unsigned int num = *p;
        *p = ((num>>16)&0x000000ff)|((num<<16)&0x00ff0000)|(num&0xff00ff00);
    }

    this->paintMap(png_matrix,width,height,x,y);
    free(png_matrix);

    return true;
}

bool LcdDevice::paintText (const char* text,int size,int len,int x,int y) {
    int w = size*len;
    int h = size;

    fontSetSize(m_font,size);
    bitmap* map = createBitmapWithInit(w,h,this->pixel_size,getColor(0,0,0,0));
    fontPrint(m_font,map,0,0,(char*)text,0xffffffff,0);

    this->paintMap(map->map,w,h,x,y);

    destroyBitmap(map);
}