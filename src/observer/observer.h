#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <algorithm>
#include <vector>
#include <mutex>

namespace obs
{

class Observer {
public:
    virtual void update() {}; //Do nothing if not overriden
};

class Subject {
private:
    std::vector<Observer*> m_observers;
    std::mutex m_lock{};

public:
    bool addObserver(Observer* observer) {
        std::lock_guard<std::mutex> lock(m_lock);

        if (std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end()) {
            m_observers.push_back(observer);
            return true;
        }
    }

    bool removeObserver(Observer* observer) {
        std::lock_guard<std::mutex> lock(m_lock);

        auto it = std::remove(m_observers.begin(), m_observers.end(), observer);
        if (it != m_observers.end()) {
            m_observers.erase(it, m_observers.end());
            return true;
        }
    }

    void notifyObservers() {
        for (Observer* observer : m_observers) {
            observer->update();
        }
    }
};

}
#endif