#ifndef _SCREEN_OBJ_H_
#define _SCREEN_OBJ_H_

#include <iostream>
#include <list>

#include "lcd.h"
#include "spng.h"


class screenObj
{
protected:
    std::list<screenObj*> m_childs{};
    screenObj* m_father{};
    LcdDevice* m_lcd;
    lcdC::bitmap* m_map{};

    unsigned char* m_bitmap{};
    size_t m_x{};
    size_t m_y{};
    size_t m_h{};
    size_t m_w{};

public:
    explicit screenObj (LcdDevice* lcd,size_t x=0,size_t y=0)
        :m_lcd(lcd),m_x{x},m_y{y} {}

    screenObj (screenObj* father,size_t x=0,size_t y=0)
        :m_father{father},m_lcd{m_father->m_lcd},m_x{father->m_x+x},m_y{father->m_y+y}
    {
        m_father->m_childs.push_back(this);
    }

    LcdDevice* getLcd () {return m_lcd;}

    virtual ~screenObj ()
    {
        if(m_map) lcdC::destroyBitmap(m_map);
        else free(m_bitmap);
        m_childs.clear();
        if (m_father) m_father->paint();
    }

    size_t getX () {return m_x;}
    size_t getY () {return m_y;}

    virtual bool isIn (size_t x,size_t y);

    void setSrc (const char* pngPath);

    void setSrc (const char* text,size_t size,size_t len);

    virtual void setSrc () {}

    virtual void paint ()
    {
        if (m_bitmap) 
        {    
            m_lcd->paintMap(m_bitmap,m_w,m_h,m_x,m_y);
        }
        if (!m_childs.empty()) {
            for (auto o:m_childs)
            {
                o->paint();
            }
        }
    }
};



#endif