#ifndef IPOMODOROSTORAGEWRITER_H_PMJNRBFY
#define IPOMODOROSTORAGEWRITER_H_PMJNRBFY

#include "core/TimeSpan.h"
#include "core/entities/Pomodoro.h"


class IPomodoroStorageWriter {
public:
    virtual ~IPomodoroStorageWriter() = default;

    virtual void save(const Pomodoro& pomodoro, long long taskId) = 0;

    virtual void remove(const Pomodoro& pomodoro) = 0;
};


#endif /* end of include guard: IPOMODOROSTORAGEWRITER_H_PMJNRBFY */
