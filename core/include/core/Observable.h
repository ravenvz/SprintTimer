#ifndef OBSERVABLE_H_3WZLFBZN
#define OBSERVABLE_H_3WZLFBZN

#include "core/Observer.h"
#include <algorithm>
#include <vector>

namespace sprint_timer {

class Observable {
public:
    virtual ~Observable() = default;

    virtual void attach(Observer& observer) { observers.push_back(&observer); }

    virtual void detach(Observer& observer)
    {
        observers.erase(
            std::find(cbegin(observers), cend(observers), &observer));
    }

    virtual void notify()
    {
        for (auto& observer : observers)
            observer->update();
    }

protected:
    std::vector<Observer*> observers;
};

} // namespace sprint_timer

#endif /* end of include guard: OBSERVABLE_H_3WZLFBZN */
