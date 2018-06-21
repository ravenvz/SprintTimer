#ifndef OBSERVER_H_GTZ5CZ1I
#define OBSERVER_H_GTZ5CZ1I

namespace core {

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
};

} // namespace core

#endif /* end of include guard: OBSERVER_H_GTZ5CZ1I */
