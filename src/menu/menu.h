#ifndef _MENU_H_
#define _MENU_H_

#include "screenObj.h"
#include "touch_screen.h"

#include <unordered_map>
#include <functional>

namespace menu{

class Menu:public screenObj
{
private:
    Tscreen* m_tscreen;
    std::unordered_map<screenObj*, std::pair<std::function<void(void*)>, void*>> m_callbacks;
public:
    explicit Menu(LcdDevice* lcd,Tscreen* tscreen,size_t x=0,size_t y=0)
        :screenObj(lcd,x,y),m_tscreen(tscreen) {}

    void registerCallback(screenObj* obj, std::function<void(void*)> callback, void* param = nullptr) {
        m_callbacks[obj] = {callback, param};
    }

    screenObj* waitForClick ();

};

}
#endif