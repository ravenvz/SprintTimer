#ifndef POMODORO_TASKSCHEDULER_H
#define POMODORO_TASKSCHEDULER_H

#include "config.h"
#include <core/TimeSpan.h>


/* Responsible for scheduling of PomodoroTimer modes.
 *
 * Description of the modes can be found in PomodoroTimer class
 * documentation.
 */
class PomodoroTimerModeScheduler {

public:
    /* Pomodoro timer modes. */
    enum class PomodoroTimerMode { Task, ShortBreak, LongBreak };

    /* Construct scheduler given number of tasks before long break. */
    explicit PomodoroTimerModeScheduler(int pomodorosBeforeBreak = 4);

    /* Set next mode. */
    void setNextMode();

    /* Cancel current mode and set mode to PomodoroTimerMode::Task. */
    void cancelMode();

    /* Return true if current mode is not a task and false otherwise. */
    bool isBreak() const;

    /* Return the number of completed pomodoros so far. */
    int numCompletedPomodoros() const;

    /* Set the number of completed pomodoros so far. */
    void setNumCompletedPomodoros(int numTasks);

    /* Toggle mode when Pomodoros are no longer interleaved with breaks. */
    void toggleInTheZoneMode();

    /* Return current mode. */
    PomodoroTimerModeScheduler::PomodoroTimerMode mode() const;

private:
    int pomodorosBeforeBreak;
    int completedPomodoros;
    bool inTheZoneMode;
    PomodoroTimerMode currentMode;
};


#endif // POMODORO_TASKSCHEDULER_H
