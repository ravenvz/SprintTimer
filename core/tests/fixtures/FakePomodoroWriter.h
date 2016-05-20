#ifndef FAKEPOMODOROWRITER_H_IHZ1QWFR
#define FAKEPOMODOROWRITER_H_IHZ1QWFR

/* Fake implementation of IPomodoroStorageWriter that exposes
 * internal storage and allows to test execute/undo. */
class FakePomodoroWriter : public IPomodoroStorageWriter {
public:
    void save(const Pomodoro& pomodoro) final { storage.store(pomodoro); }

    void remove(const Pomodoro& pomodoro) final { storage.remove(pomodoro.uuid()); }

    FakePomodoroStorage storage;
};


#endif /* end of include guard: FAKEPOMODOROWRITER_H_IHZ1QWFR */
