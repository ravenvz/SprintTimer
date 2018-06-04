/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "core/CountdownTimer.h"
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

namespace SprintTimerCore {

class TimerState;

class StatefulTimer : public IStatefulTimer {
    friend class TimerState;
    friend class ShortBreak;
    friend class LongBreak;
    friend class RunningSprint;
    friend class Zone;
    friend class Idle;
    friend class SprintFinished;

public:
    StatefulTimer(
        std::function<void(std::chrono::seconds timeLeft)> tickCallback,
        std::function<void(IStatefulTimer::StateId state)> onStateChangedCallback,
        std::chrono::seconds tickPeriod,
        const IConfig& applicationSettings);

    ~StatefulTimer() override = default;

    StatefulTimer(StatefulTimer&&) = delete;
    StatefulTimer& operator=(StatefulTimer&&) = delete;

    StatefulTimer(const StatefulTimer&) = delete;
    StatefulTimer& operator=(const StatefulTimer&) = delete;

    void start() override;

    void cancel() override;

    std::chrono::seconds currentDuration() const override;

    void toggleInTheZoneMode() override;

    std::vector<TimeSpan> completedSprints() const override;

    void clearSprintsBuffer() override;

    void setNumFinishedSprints(int num);

    int numFinishedSprints() const;

protected:
    TimerState* currentState;

    virtual /* Transition to another state while exiting current state properly. */
    void transitionToState(TimerState& state);

private:
    const IConfig& applicationSettings;
    const std::chrono::seconds tickInterval;
    std::function<void(std::chrono::seconds timeLeft)> onTickCallback;
    std::function<void(IStatefulTimer::StateId)> onStateChangedCallback;
    std::unique_ptr<CountdownTimer> countdownTimer;
    DateTime mStart;
    int finishedSprints{0};
    std::vector<TimeSpan> buffer;

    /* Jump to another state immediately ignoring proper exit from current state */
    void jumpToState(TimerState& state);
    bool longBreakConditionMet() const;
    void onTimerRunout();
    void notifyStateChanged(IStatefulTimer::StateId stateId);
    void onTimerTick(CountdownTimer::TickPeriod timeLeft);
    void startCountdown();
};

class TimerState {
public:
    virtual ~TimerState() = default;
    virtual void enter(StatefulTimer& timer) const = 0;
    virtual void cancelExit(StatefulTimer& timer) = 0;
    virtual void normalExit(StatefulTimer& timer) const = 0;
    virtual void setNextState(StatefulTimer& timer) = 0;
    virtual void toggleZoneMode(StatefulTimer& timer);
    virtual std::chrono::seconds duration(const StatefulTimer& timer) const;
    virtual void onTimerFinished(StatefulTimer& timer);
};

class Idle final : public TimerState {
public:
    void enter(StatefulTimer& timer) const override;
    void cancelExit(StatefulTimer& timer) override;
    void normalExit(StatefulTimer& timer) const override;
    void setNextState(StatefulTimer& timer) override;
};

class RunningSprint final : public TimerState {
public:
    void enter(StatefulTimer& timer) const override;
    void cancelExit(StatefulTimer& timer) override;
    void normalExit(StatefulTimer& timer) const override;
    void setNextState(StatefulTimer& timer) override;
    std::chrono::seconds duration(const StatefulTimer& timer) const override;
    void toggleZoneMode(StatefulTimer& timer) override;
    void onTimerFinished(StatefulTimer& timer) override;
};

class ShortBreak final : public TimerState {
public:
    void enter(StatefulTimer& timer) const override;
    void cancelExit(StatefulTimer& timer) override;
    void normalExit(StatefulTimer& timer) const override;
    void setNextState(StatefulTimer& timer) override;
    std::chrono::seconds duration(const StatefulTimer& timer) const override;
    void onTimerFinished(StatefulTimer& timer) override;
};

class LongBreak final : public TimerState {
public:
    void enter(StatefulTimer& timer) const override;
    void cancelExit(StatefulTimer& timer) override;
    void normalExit(StatefulTimer& timer) const override;
    void setNextState(StatefulTimer& timer) override;
    std::chrono::seconds duration(const StatefulTimer& timer) const override;
    void onTimerFinished(StatefulTimer& timer) override;
};

class Zone final : public TimerState {
public:
    void enter(StatefulTimer& timer) const override;
    void cancelExit(StatefulTimer& timer) override;
    void normalExit(StatefulTimer& timer) const override;
    void setNextState(StatefulTimer& timer) override;
    std::chrono::seconds duration(const StatefulTimer& timer) const override;
    void toggleZoneMode(StatefulTimer& timer) override;
    void onTimerFinished(StatefulTimer& timer) override;
};

class SprintFinished final : public TimerState {
public:
    void enter(StatefulTimer& timer) const override;
    void cancelExit(StatefulTimer& timer) override;
    void normalExit(StatefulTimer& timer) const override;
    void setNextState(StatefulTimer& timer) override;
};

} // namespace SprintTimerCore

#endif /* end of include guard: TASKRUNNER_H_9VSDY5UR */
