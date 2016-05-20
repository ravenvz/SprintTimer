#ifndef IPOMODOROSTORAGEWRITER_H_PMJNRBFY
#define IPOMODOROSTORAGEWRITER_H_PMJNRBFY

#include "TimeSpan.h"
#include "entities/Pomodoro.h"


class IPomodoroStorageWriter {
public:
    virtual ~IPomodoroStorageWriter() = default;

    virtual void save(const Pomodoro& pomodoro) = 0;

    virtual void remove(const Pomodoro& pomodoro) = 0;
};


#endif /* end of include guard: IPOMODOROSTORAGEWRITER_H_PMJNRBFY */
