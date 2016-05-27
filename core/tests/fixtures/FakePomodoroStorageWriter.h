#ifndef FAKEPOMODOROWRITER_H_IHZ1QWFR
#define FAKEPOMODOROWRITER_H_IHZ1QWFR

#include "core/IPomodoroStorageWriter.h"
#include "FakeStorage.h"

/* Fake implementation of IPomodoroStorageWriter that exposes
 * internal storage and allows to test execute/undo. */
class FakePomodoroWriter : public IPomodoroStorageWriter {
public:
    FakePomodoroWriter(FakeStorage<Pomodoro>& storage)
        : storage{storage}
    {
    }

    void save(const Pomodoro& pomodoro) final { storage.store(pomodoro); }

    void remove(const Pomodoro& pomodoro) final
    {
        storage.remove(pomodoro.uuid());
    }

    FakeStorage<Pomodoro>& storage;
};


#endif /* end of include guard: FAKEPOMODOROWRITER_H_IHZ1QWFR */
