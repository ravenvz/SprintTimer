/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#ifndef TASKRUNNER_H_9VSDY5UR
#define TASKRUNNER_H_9VSDY5UR

#include "date_wrapper/DateTime.h"
#include "core/IConfig.h"
#include "core/IStatefulTimer.h"
#include "date_wrapper/TimeSpan.h"
#include "core/Timer.h"
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

namespace {
    constexpr int millisecondsInMinute{60'000};
}

class TimerState;

class StatefulTimer : public IStatefulTimer {
    friend class TimerState;
    friend class ShortBreak;
    friend class LongBreak;
    friend class RunningSprint;
    friend class Zone;
    friend class Idle;
    friend class Finished;

public:
    /* Construct StatefulTimer given callback function,
     * tick period and reference to application settings.
     *
     * When timer is running, callback function will be executed after
     * each tick period until time runs out or timer is cancelled. */
    StatefulTimer(
        std::function<void(long long timeLeft)> tickCallback,
        std::function<void(IStatefulTimer::State state)> onStateChangedCallback,
        long tickPeriodInMillisecs,
        const IConfig& applicationSettings);

    ~StatefulTimer() = default;

    StatefulTimer(StatefulTimer&&) = delete;
    StatefulTimer& operator=(StatefulTimer&&) = delete;

    StatefulTimer(const StatefulTimer&) = delete;
    StatefulTimer& operator=(const StatefulTimer&) = delete;

    /* Start timer countdown for current state.
     *
     * tickCallback(timeLeft) function will be called each tick until time
     * for the task runs out or cancel() method is called. */
    void start() final;

    /* Cancel current. */
    void cancel() final;

    /* Return task duration in minutes.
     *
     * Duration is obtained from application config. */
    int currentDuration() const final;

    /* Return current state. */
    IStatefulTimer::State state() const final;

    /* Toggles the InTheZone mode when tasks are no longer interleaved with
     * breaks and scheduled one after another without breaks. */
    void toggleInTheZoneMode() final;

    /* Return vector of intervals that are currently registered by timer. */
    std::vector<TimeSpan> completedTaskIntervals() const final;

    /* Clear registered time intervals. */
    void clearIntervalsBuffer() final;

    /* Set number of sprints finished so far. */
    void setNumFinishedSprints(int num);

    /* Return number of finished sprints. */
    int numFinishedSprints() const;


protected:
    std::chrono::milliseconds millisecondsLeft;
    std::unique_ptr<TimerState> idleState;
    std::unique_ptr<TimerState> sprintState;
    std::unique_ptr<TimerState> shortBreakState;
    std::unique_ptr<TimerState> longBreakState;
    std::unique_ptr<TimerState> zoneState;
    std::unique_ptr<TimerState> finishedState;
    TimerState* currentState;

private:
    const IConfig& applicationSettings;
    const std::chrono::milliseconds tickInterval;
    std::function<void(long long timeLeft)> onTickCallback;
    std::function<void(IStatefulTimer::State)> onStateChangedCallback;
    std::unique_ptr<Timer> timerPtr;
    DateTime mStart;
    int finishedSprints{0};
    std::vector<TimeSpan> buffer;

    void onTimerTick();
    void resetTimer();
};

/* Base class for sprint timer states. */
class TimerState {
public:
    TimerState(StatefulTimer& statefulTimer)
        : timer{statefulTimer}
    {
    }

    virtual ~TimerState() = default;

    virtual IStatefulTimer::State state() const = 0;

    virtual void start() {}

    virtual void setNextState(){};

    virtual void notifyStateChanged(IStatefulTimer::State state);

    virtual void cancel(){};

    virtual void toggleZoneMode(){};

    virtual int duration() const = 0;

protected:
    StatefulTimer& timer;
};

class Idle final : public TimerState {
public:
    Idle(StatefulTimer& timer);

    IStatefulTimer::State state() const;

    void setNextState();

    int duration() const;
};

class RunningSprint final : public TimerState {
public:
    RunningSprint(StatefulTimer& timer);

    IStatefulTimer::State state() const;

    void start();

    void setNextState();

    void cancel();

    int duration() const;

    void toggleZoneMode();
};

class ShortBreak final : public TimerState {
public:
    ShortBreak(StatefulTimer& timer);

    IStatefulTimer::State state() const;

    void start();

    void setNextState();

    void cancel();

    int duration() const;
};

class LongBreak final : public TimerState {
public:
    LongBreak(StatefulTimer& timer);

    IStatefulTimer::State state() const;

    void start();

    void setNextState();

    void cancel();

    int duration() const;
};

class Zone final : public TimerState {
public:
    Zone(StatefulTimer& timer);

    void start();

    IStatefulTimer::State state() const;

    void setNextState();

    int duration() const;

    void toggleZoneMode();
};

class Finished final : public TimerState {
public:
    Finished(StatefulTimer& timer);

    IStatefulTimer::State state() const;

    void setNextState();

    void cancel();

    int duration() const;
};

#endif /* end of include guard: TASKRUNNER_H_9VSDY5UR */
