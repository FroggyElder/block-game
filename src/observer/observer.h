#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <algorithm>
#include <vector>

namespace obs
{

class Observer {
public:
    virtual void update() {}; //Do nothing if not overriden
};

class Subject {
private:
    std::vector<Observer*> observers;

public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyObservers() {
        for (Observer* observer : observers) {
            observer->update();
        }
    }
};

}
#endif