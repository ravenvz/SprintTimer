/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#ifndef TASKRUNNER_H_9VSDY5UR
#define TASKRUNNER_H_9VSDY5UR

#include "core/DateTime.h"
#include "core/IConfig.h"
#include "core/IPomodoroTimer.h"
#include "core/TimeSpan.h"
#include "core/Timer.h"
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

class PomodoroTimerState;

class PomodoroTimer : public IPomodoroTimer {
    friend class PomodoroTimerState;
    friend class ShortBreakState;
    friend class LongBreakState;
    friend class TaskState;
    friend class ZoneState;
    friend class IdleState;
    friend class FinishedState;

public:
    /* Construct PomodoroTimer given callback function,
     * tick period and reference to application settings.
     *
     * When timer is running, callback function will be executed after
     * each tick period until time runs out or timer is cancelled. */
    PomodoroTimer(
        std::function<void(long timeLeft)> tickCallback,
        std::function<void(IPomodoroTimer::State state)> onStateChangedCallback,
        long tickPeriodInMillisecs,
        const IConfig& applicationSettings);

    ~PomodoroTimer() = default;

    PomodoroTimer(PomodoroTimer&&) = delete;
    PomodoroTimer& operator=(PomodoroTimer&&) = delete;

    PomodoroTimer(const PomodoroTimer&) = delete;
    PomodoroTimer& operator=(const PomodoroTimer&) = delete;

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
    IPomodoroTimer::State state() const final;

    /* Toggles the InTheZone mode when tasks are no longer interleaved with
     * breaks and scheduled one after another without breaks. */
    void toggleInTheZoneMode() final;

    /* Return vector of intervals that are currently registered by timer. */
    std::vector<TimeSpan> completedTaskIntervals() const final;

    /* Clear registered time intervals. */
    void clearIntervalsBuffer() final;

    /* Set number of pomodoros completed so far. */
    void setNumCompletedPomodoros(int num);

    /* Return number of pomodoros completed so far. */
    int numCompletedPomodoros() const;


protected:
    const int millisecondsInMinute{60000};
    std::chrono::milliseconds millisecondsLeft;
    std::unique_ptr<PomodoroTimerState> idleState;
    std::unique_ptr<PomodoroTimerState> pomodoroState;
    std::unique_ptr<PomodoroTimerState> shortBreakState;
    std::unique_ptr<PomodoroTimerState> longBreakState;
    std::unique_ptr<PomodoroTimerState> zoneState;
    std::unique_ptr<PomodoroTimerState> finishedState;
    PomodoroTimerState* currentState;

private:
    const IConfig& applicationSettings;
    const std::chrono::milliseconds tickInterval;
    std::function<void(long timeLeft)> onTickCallback;
    std::function<void(IPomodoroTimer::State)> onStateChangedCallback;
    std::unique_ptr<Timer> timerPtr;
    DateTime mStart;
    int completedPomodoros{0};
    std::vector<TimeSpan> buffer;

    void onTimerTick();
    void resetTimer();
};

/* Base class for pomodoro timer states. */
class PomodoroTimerState {
public:
    PomodoroTimerState(PomodoroTimer& pomodoroTimer)
        : timer{pomodoroTimer}
    {
    }

    virtual ~PomodoroTimerState() = default;

    virtual IPomodoroTimer::State state() const = 0;

    virtual void start() {}

    virtual void setNextState(){};

    virtual void notifyStateChanged(IPomodoroTimer::State state);

    virtual void cancel(){};

    virtual void toggleZoneMode(){};

    virtual int duration() const = 0;

protected:
    PomodoroTimer& timer;
};

class IdleState final : public PomodoroTimerState {
public:
    IdleState(PomodoroTimer& timer);

    IPomodoroTimer::State state() const;

    void setNextState();

    int duration() const;
};

class TaskState final : public PomodoroTimerState {
public:
    TaskState(PomodoroTimer& timer);

    IPomodoroTimer::State state() const;

    void start();

    void setNextState();

    void cancel();

    int duration() const;

    void toggleZoneMode();
};

class ShortBreakState final : public PomodoroTimerState {
public:
    ShortBreakState(PomodoroTimer& timer);

    IPomodoroTimer::State state() const;

    void start();

    void setNextState();

    void cancel();

    int duration() const;
};

class LongBreakState final : public PomodoroTimerState {
public:
    LongBreakState(PomodoroTimer& timer);

    IPomodoroTimer::State state() const;

    void start();

    void setNextState();

    void cancel();

    int duration() const;
};

class ZoneState final : public PomodoroTimerState {
public:
    ZoneState(PomodoroTimer& timer);

    void start();

    IPomodoroTimer::State state() const;

    void setNextState();

    int duration() const;

    void toggleZoneMode();
};

class FinishedState final : public PomodoroTimerState {
public:
    FinishedState(PomodoroTimer& timer);

    IPomodoroTimer::State state() const;

    void setNextState();

    void cancel();

    int duration() const;
};

#endif /* end of include guard: TASKRUNNER_H_9VSDY5UR */
