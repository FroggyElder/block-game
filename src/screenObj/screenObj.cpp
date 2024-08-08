#include "screenObj.h"

void screenObj::setSrc (const char* pngPath)
{
    lcdC::read_png_to_argb(pngPath,&m_bitmap,&m_w,&m_h);

    for (int i=0;i<(m_w*m_h);i++)
    {
        unsigned int* p = (unsigned int*)m_bitmap + i;
        unsigned int num = *p;
        *p = ((num>>16)&0x000000ff)|((num<<16)&0x00ff0000)|(num&0xff00ff00);
    }
}

void screenObj::setSrc (const char* text,size_t size,size_t len)
{
    m_w = size*len;
    m_h = size;

    fontSetSize(m_lcd->getFont(),size);
    m_map = lcdC::createBitmapWithInit(m_w,m_h,m_lcd->getPixelSize(),getColor(0,0,0,0));
    fontPrint(m_lcd->getFont(),m_map,0,0,(char*)text,0xffffffff,0);

    m_bitmap = m_map->map;
}

bool screenObj::isIn (size_t x,size_t y)
{
    return (x>m_x&&x<m_x+m_w)&&(y>m_y&&y<m_y+m_h);
}