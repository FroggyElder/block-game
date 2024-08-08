#include "menu.h"
using namespace menu;

screenObj* Menu::waitForClick ()
 {
    if (m_childs.empty()) return nullptr;

    while (m_tscreen->getAction() != Tscreen::Action::tap);

    int inputX = m_tscreen->getStatus().x;
    int inputY = m_tscreen->getStatus().y;

    for (auto child : m_childs) {
        if (child->isIn(inputX, inputY)) {
            auto it = m_callbacks.find(child);
            if (it != m_callbacks.end()) {
                auto& callback = it->second.first;
                auto& param = it->second.second;
                callback(param);
            }
            return child;
        }
    }

    return nullptr;
}