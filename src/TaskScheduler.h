#ifndef POMODORO_TASKSCHEDULER_H
#define POMODORO_TASKSCHEDULER_H

#include <chrono>

//class Schedulable {
//
//public:
//    virtual bool isBreak() = 0;
//    void start() {
//        start_time = std::chrono::system_clock::now();
//    };
//    void finish() {
//        finish_time = std::chrono::system_clock::now();
//    }
//    void cancel() {
//        canceled = true;
//    }
//    unsigned getDuration() {
//        return durationInMinutes;
//    }
//
//protected:
//    unsigned durationInMinutes;
//    std::chrono::system_clock::time_point start_time;
//    std::chrono::system_clock::time_point finish_time;
//    bool canceled = false;
//};
//
//class Task : public Schedulable {
//
//public:
//    Task(unsigned durationInMinutes) :
//        durationInMinutes(durationInMinutes)
//    {}
//    bool isBreak() override {
//        return false;
//    }
//};
//
//class Break : public Schedulable {
//
//public:
//    Break(unsigned durationInMinutes);
//    bool isBreak() override {
//        return true;
//    }
//};

struct Config {
    Config(unsigned pomodoroDuration, unsigned breakDuration, unsigned longBreakDuration, unsigned tasksBeforeBreak) :
            pomodoroDuration(pomodoroDuration),
            breakDuration(breakDuration),
            longBreakDuration(longBreakDuration),
            tasksBeforeBreak(tasksBeforeBreak)
    {}
    const unsigned pomodoroDuration;
    const unsigned breakDuration;
    const unsigned longBreakDuration;
    const unsigned tasksBeforeBreak;
};


class TaskScheduler {

public:
    TaskScheduler(Config& config, unsigned completedPomodoros);
    void startTask();
    void finishTask();
    void cancelTask();
    void setNextState();
    unsigned getTaskDuration();
    bool isBreak();
    unsigned getNumCompletedTasks();

private:
    Config& config;
    unsigned completedTasks;
    bool inTheZoneMode;
    enum class TaskState { TASK, SHORT_BREAK, LONG_BREAK  };
    TaskState currentState;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point finishTime;
};



#endif //POMODORO_TASKSCHEDULER_H
