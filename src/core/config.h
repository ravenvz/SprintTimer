#ifndef POMODORO_CONFIG_H
#define POMODORO_CONFIG_H

#include <QVariant>
#include <QSettings>
#include <QDebug>


class IConfig {

public:
    virtual ~IConfig() = default;
    virtual int pomodoroDuration() const = 0;
    virtual void setPomodoroDuration(int pomodoroDuration) = 0;
    virtual int shortBreakDuration() const = 0;
    virtual void setShortBreakDuration(int shortBreakDuration) = 0;
    virtual int longBreakDuration() const = 0;
    virtual void setLongBreakDuration(int longBreakDuration) = 0;
    virtual int numTasksBeforeBreak() const = 0;
    virtual void setTasksBeforeBreak(int tasksBeforeBreak) = 0;
    virtual bool soundIsEnabled() const = 0;
    virtual void setPlaySound(bool playSound) = 0;
    virtual int soundVolume() const = 0;
    // TODO split into separate group
    virtual void setSoundVolume(int soundVolume) = 0;
    virtual int dailyPomodorosGoal() const = 0;
    virtual void setDailyPomodorosGoal(int dailyPomodorosGoal) = 0;
    virtual int weeklyPomodorosGoal() const = 0;
    virtual void setWeeklyPomodorosGoal(int weeklyPomodorosGoal) = 0;
    virtual int monthlyPomodorosGoal() const = 0;
    virtual void setMonthlyPomodorosGoal(int monthlyPomodorosGoal) = 0;
};


class Config : public IConfig
{

public:

    // Config() {
    //
    // }

    int pomodoroDuration() const override {
        return settings.value("pomodoroDuration").toInt();
    }

    void setPomodoroDuration(int pomodoroDuration) override {
        Config::settings.setValue("pomodoroDuration", QVariant(pomodoroDuration));
    }

    int shortBreakDuration() const override {
        return settings.value("shortBreakDuration").toInt();
    }

    void setShortBreakDuration(int shortBreakDuration) override {
        Config::settings.setValue("shortBreakDuration", QVariant(shortBreakDuration));
    }

    int longBreakDuration() const override {
        return settings.value("longBreakDuration").toInt();
    }

    void setLongBreakDuration(int longBreakDuration) override {
        Config::settings.setValue("longBreakDuration", QVariant(longBreakDuration));
    }

    int numTasksBeforeBreak() const override {
        return settings.value("tasksBeforeBreak").toInt();
    }

    void setTasksBeforeBreak(int tasksBeforeBreak) override {
        Config::settings.setValue("tasksBeforeBreak", QVariant(tasksBeforeBreak));
    }

    bool soundIsEnabled() const override {
        return settings.value("playSound").toBool();
    }

    void setPlaySound(bool playSound) override {
        Config::settings.setValue("playSound", QVariant(playSound));
    }

    int soundVolume() const override {
        return settings.value("soundVolume").toInt();
    }

    void setSoundVolume(int soundVolume) override {
        Config::settings.setValue("soundVolume", QVariant(soundVolume));
    }

    int dailyPomodorosGoal() const override {
        return settings.value("dailyPomodorosGoal").toInt();
    }

    void setDailyPomodorosGoal(int dailyPomodorosGoal) override {
        Config::settings.setValue("dailyPomodorosGoal", QVariant(dailyPomodorosGoal));
    }

    int weeklyPomodorosGoal() const override {
        return settings.value("weeklyPomodorosGoal").toInt();
    }

    void setWeeklyPomodorosGoal(int weeklyPomodorosGoal) override {
        Config::settings.setValue("weeklyPomodorosGoal", QVariant(weeklyPomodorosGoal));
    }

    int monthlyPomodorosGoal() const override {
        return settings.value("monthlyPomodorosGoal").toInt();
    }

    void setMonthlyPomodorosGoal(int monthlyPomodorosGoal) override {
        Config::settings.setValue("monthlyPomodorosGoal", QVariant(monthlyPomodorosGoal));
    }

private:
    QSettings settings;
};

#endif //POMODORO_CONFIG_H
