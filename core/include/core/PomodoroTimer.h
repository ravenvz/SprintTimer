#ifndef TASKRUNNER_H_9VSDY5UR
#define TASKRUNNER_H_9VSDY5UR

// #include "../../lib/date/date.h"
#include "core/DateTime.h"
#include "core/TimeSpan.h"
#include "core/Timer.h"
#include "core/IConfig.h"
#include <chrono>
#include <memory>

class PomodoroTimerState;

/* Responsible for running pomodoro timer.
 *
 * Pomodoro timer is just a stopwatch that can have three states, that
 * differ only in length of their initial time and a special InTheZone mode.
 *
 * These states are PomodoroState, ShortBreakState and LongBreakState.
 * Their duration and the order they should change each other is defined
 * in application settings.
 * Default values for Pomodoro Technique are:
 *      25 minutes for Pomodoro;
 *      5 minutes for ShortBreak;
 *      15 minutes for LongBreak.
 * Each Pomodoro is followed by ShortBreak. Each 4 (by default) Tasks are
 * followed by LongBreak.
 *
 * When PomodoroTimer is in the special InTheZone mode, Tasks are no longer
 * interleaved with breaks, but follow each other consecutively. This mode can
 * be toggled with toggleInTheZoneMode() method.
 *
 * When PomodoroTimer is running, it executes tickCallback(timeLeft) callback,
 * that should be provided to constructor, where timeLeft - is total time
 * left in milliseconds.
 *
 * PomodoroTimer manages it's state schedule internally and does not provide
 * public methods to alter states directly. Instead, clients of the class should
 * rely on indirect methods for changing state by calling run(), cancel() and
 * finish() methods in the following manner:
 *
 *     run() method is called and PomodoroTimer starts running. When total time
 *     runs out, internal timer is stopped and tickCallback(0) is called.
 *     The only way for client code to know that PomodoroTimer is stopped
 *     normally is to handle tickCallback(0). This is done deliberately, so that
 *     clients can decide what to do with this information - they can still
 *     cancel timer at this point or call finish() that returns TimeSpan
 *     instance.
 *     Note, that finish point of the TimeSpan instance will be recorded when
 *     finish() is called, not when timer runs out.
 *
 *     If cancel() method is called at any point between run() and finish(),
 *     current mode is considered cancelled and next state is set to
 *     PomodoroState.
 *
 *     When finish() method is called, current state is considered to be
 * finished
 *     normally and next state is set.
 */
class PomodoroTimer {
    friend class ShortBreakState;
    friend class LongBreakState;
    friend class PomodoroState;

public:
    /* Construct PomodoroTimer given callback function,
     * tick period and reference to application settings.
     *
     * When task is running, callback function will be executed after
     * each tick period until time runs out or timer is cancelled. */
    PomodoroTimer(std::function<void(long timeLeft)> tickCallback,
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
     * for the task runs out or cancel() or finish() methods are called. */
    void run();

    /* Cancel current mode.
     *
     * This action stops countdown timer. Current state is considered
     * interrupted and next state is unconditionally set to PomodoroState. */
    void cancel();

    /* Return TimeSpan and set next mode
     *
     * Note that timeSpan's finish time will be recorded only after this method
     * is called, not when timer runs out.
     *
     * If called when timer is still running, timer will be stopped.  */
    TimeSpan finish();

    /* Return task duration in minutes.
     *
     * Duration is obtained from application config. */
    int taskDuration();

    /* Return true if current task type is ShortBrake or
     * PomodoroTimerModeScheduler
     * and false otherwise. */
    bool isBreak() const;

    /* Toggles the InTheZone mode when tasks are no longer interleaved with
     * breaks and scheduled one after another without breaks. */
    void toggleInTheZoneMode();

    /* Set number of pomodoros completed so far. */
    void setNumCompletedPomodoros(int num);

    /* Return number of pomodoros completed so far. */
    int numCompletedPomodoros() const;

private:
    const IConfig& applicationSettings;
    const std::chrono::milliseconds tickInterval;
    std::function<void(long timeLeft)> onTickCallback;
    std::unique_ptr<Timer> timerPtr;
    DateTime mStart;
    std::chrono::milliseconds currentTaskDuration;
    const int millisecondsInMinute{60000};
    int completedPomodoros{0};
    bool running{false};
    bool inTheZoneMode{false};
    std::unique_ptr<PomodoroTimerState> shortBreakState;
    std::unique_ptr<PomodoroTimerState> longBreakState;
    std::unique_ptr<PomodoroTimerState> pomodoroState;
    PomodoroTimerState* currentState;

    void onTimerTick();
};

/* Base class for pomodoro timer states. */
class PomodoroTimerState {
public:
    PomodoroTimerState(PomodoroTimer& pomodoroTimer)
        : timer{pomodoroTimer}
    {
    }

    virtual ~PomodoroTimerState() = default;

    virtual void setNextState() = 0;

    virtual void cancel() = 0;

    virtual bool isBreak() const = 0;

    virtual int duration() const = 0;

protected:
    PomodoroTimer& timer;
};

class ShortBreakState : public PomodoroTimerState {
public:
    ShortBreakState(PomodoroTimer& timer)
        : PomodoroTimerState{timer}
    {
    }

    void setNextState() final
    {
        timer.currentState = timer.pomodoroState.get();
    }

    void cancel() final { timer.currentState = timer.pomodoroState.get(); }

    bool isBreak() const final { return true; }

    int duration() const final
    {
        return timer.applicationSettings.shortBreakDuration();
    }
};

class LongBreakState : public PomodoroTimerState {
public:
    LongBreakState(PomodoroTimer& timer)
        : PomodoroTimerState{timer}
    {
    }

    void setNextState() final
    {
        timer.currentState = timer.pomodoroState.get();
    }

    void cancel() final { timer.currentState = timer.pomodoroState.get(); }

    bool isBreak() const final { return true; }

    int duration() const final
    {
        return timer.applicationSettings.longBreakDuration();
    }
};

class PomodoroState : public PomodoroTimerState {
public:
    PomodoroState(PomodoroTimer& timer)
        : PomodoroTimerState{timer}
    {
    }

    void setNextState() final
    {
        if (timer.inTheZoneMode) {
            timer.currentState = timer.pomodoroState.get();
        }
        else if (timer.completedPomodoros
                     % timer.applicationSettings.numPomodorosBeforeBreak()
                 == 0) {
            timer.currentState = timer.longBreakState.get();
        }
        else {
            timer.currentState = timer.shortBreakState.get();
        }
    }

    void cancel() final { timer.currentState = timer.pomodoroState.get(); }

    bool isBreak() const final { return false; }

    int duration() const final
    {
        return timer.applicationSettings.pomodoroDuration();
    }
};

#endif /* end of include guard: TASKRUNNER_H_9VSDY5UR */
