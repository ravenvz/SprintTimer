#ifndef TASKRUNNER_H_9VSDY5UR
#define TASKRUNNER_H_9VSDY5UR

#include "PomodoroTimerModeScheduler.h"
#include "TimeSpan.h" 
#include "Timer.h"
#include "config.h"
#include "date/date.h"
#include <chrono>
#include <memory>

/* Responsible for running pomodoro timer.
 *
 * Pomodoro timer is just a stopwatch that can have three modes, that
 * differ only in length of their initial time and a special InTheZone mode.
 *
 * These modes are Task, ShortBreak and LongBreak. Their duration and the order
 * they should change each other is defined in application settings.
 * Default values for Pomodoro Technique are:
 *      25 minutes for Task;
 *      5 minutes for ShortBreak;
 *      15 minutes for LongBreak.
 * Each Task is followed by ShortBreak. Each 4 (by default) Tasks are followed
 * by LongBreak.
 *
 * When PomodoroTimer is in the special InTheZone mode, Tasks are no longer
 * interleaved with breaks, but follow each other consecutively. This mode can
 * be toggled with toggleInTheZoneMode() method.
 *
 * When PomodoroTimer is running, it executes tickCallback(timeLeft) callback,
 * that should be provided to constructor, where timeLeft - is total time
 * left in milliseconds.
 *
 * PomodoroTimer manages it's mode schedule internally and does not provide
 * public methods to alter modes directly. Instead, clients of the class should
 * rely on indirect methods for changing state by calling run(), cancel() and
 * finish() methods in the following manner:
 *
 *     run() method is called and PomodoroTimer starts running. When total time
 *     runs out, internal timer is stopped and tickCallback(0) is called.
 *     The only way for client code to know that PomodoroTimer is stopped
 *     normally is to handle tickCallback(0). This is done deliberately, so that
 *     clients can decide what to do with this information - they can still
 * cancel
 *     timer at this point or call finish() that returns TimeSpan instance.
 *     Note, that finish point of the TimeSpan instance will be recorded when
 *     finish() is called, not when timer runs out.
 *
 *     If cancel() method is called at any point between run() and finish(),
 *     current mode is considered cancelled and next mode is set to Task.
 *
 *     When finish() method is called, current mode is considered to be finished
 *     normally and next mode is set.
 *
 */
class PomodoroTimer {
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

    /* Start task.
     *
     * tickCallback(timeLeft) function will be called each tick until time
     * for the task runs out or cancel() or finish() methods are called. */
    void run();

    /* Cancel current mode and switch to Task mode. */
    void cancel();

    /* Return TimeSpan and set next mode
     *
     * Note that timeSpan's finish time will be recorded only after this method
     * is called, not when timer runs out. */
    TimeSpan finish();

    /* Return task duration in minutes.
     *
     * Duration is obtained from application config. */
    int taskDuration();

    /* Return true if current task type is ShortBrake or PomodoroTimerModeScheduler
     * and false otherwise. */
    bool isBreak() const;

    /* Toggles the InTheZone mode when tasks are no longer interleaved with
     * breaks and scheduled one after another without breaks. */
    void toggleInTheZoneMode();

private:
    const IConfig& applicationSettings;
    PomodoroTimerModeScheduler taskScheduler;
    const std::chrono::milliseconds tickInterval;
    std::function<void(long timeLeft)> onTickCallback;
    std::unique_ptr<Timer> timerPtr;
    DateTime mStart;
    DateTime mFinish;
    std::chrono::milliseconds currentTaskDuration;
    const int millisecondsInMinute{60000};

    void onTimerTick();
};

#endif /* end of include guard: TASKRUNNER_H_9VSDY5UR */
