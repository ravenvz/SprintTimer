#ifndef OBSERVER_H_GTZ5CZ1I
#define OBSERVER_H_GTZ5CZ1I

namespace sprint_timer {

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: OBSERVER_H_GTZ5CZ1I */
