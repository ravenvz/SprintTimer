#ifndef IPOMODOROTIMER_H_D3DG0N7C
#define IPOMODOROTIMER_H_D3DG0N7C

#include "core/TimeSpan.h"

class IPomodoroTimer {
public:
    virtual ~IPomodoroTimer() = default;

    virtual void run() = 0;

    virtual void cancel() = 0;

    virtual TimeSpan finish() = 0;

    virtual int taskDuration() = 0;

    virtual bool isBreak() = 0;

    virtual void toggleInTheZoneMode() = 0;
};

#endif /* end of include guard: IPOMODOROTIMER_H_D3DG0N7C */
