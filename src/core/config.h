#ifndef POMODORO_CONFIG_H
#define POMODORO_CONFIG_H

#include <QVariant>
#include <QSettings>
#include <QDebug>


class IConfig {

public:
    virtual unsigned int getPomodoroDuration() const = 0;
    virtual void setPomodoroDuration(unsigned int pomodoroDuration) = 0;
    virtual unsigned int getShortBreakDuration() const = 0;
    virtual void setShortBreakDuration(unsigned int shortBreakDuration) = 0;
    virtual unsigned int getLongBreakDuration() const = 0;
    virtual void setLongBreakDuration(unsigned int longBreakDuration) = 0;
    virtual unsigned int getTasksBeforeBreak() const = 0;
    virtual void setTasksBeforeBreak(unsigned int tasksBeforeBreak) = 0;
    virtual bool soundIsEnabled() const = 0;
    virtual void setPlaySound(bool playSound) = 0;
    virtual int getSoundVolume() const = 0;
    // TODO split into separate group
    virtual void setSoundVolume(int soundVolume) = 0;
    virtual unsigned int getDailyPomodorosGoal() const = 0;
    virtual void setDailyPomodorosGoal(unsigned dailyPomodorosGoal) = 0;
    virtual unsigned int getWeeklyPomodorosGoal() const = 0;
    virtual void setWeeklyPomodorosGoal(unsigned weeklyPomodorosGoal) = 0;
    virtual unsigned int getMonthlyPomodorosGoal() const = 0;
    virtual void setMonthlyPomodorosGoal(unsigned monthlyPomodorosGoal) = 0;
};


class Config : public IConfig
{

public:

    Config() {

    }

    unsigned int getPomodoroDuration() const override {
        return settings.value("pomodoroDuration").toUInt();
    }

    void setPomodoroDuration(unsigned int pomodoroDuration) override {
        Config::settings.setValue("pomodoroDuration", QVariant(pomodoroDuration));
    }

    unsigned int getShortBreakDuration() const override {
        return settings.value("shortBreakDuration").toUInt();
    }

    void setShortBreakDuration(unsigned int shortBreakDuration) override {
        Config::settings.setValue("shortBreakDuration", QVariant(shortBreakDuration));
    }

    unsigned int getLongBreakDuration() const override {
        return settings.value("longBreakDuration").toUInt();
    }

    void setLongBreakDuration(unsigned int longBreakDuration) override {
        Config::settings.setValue("longBreakDuration", QVariant(longBreakDuration));
    }

    unsigned int getTasksBeforeBreak() const override {
        return settings.value("tasksBeforeBreak").toUInt();
    }

    void setTasksBeforeBreak(unsigned int tasksBeforeBreak) override {
        Config::settings.setValue("tasksBeforeBreak", QVariant(tasksBeforeBreak));
    }

    bool soundIsEnabled() const override {
        return settings.value("playSound").toBool();
    }

    void setPlaySound(bool playSound) override {
        Config::settings.setValue("playSound", QVariant(playSound));
    }

    int getSoundVolume() const override {
        return settings.value("soundVolume").toInt();
    }

    void setSoundVolume(int soundVolume) override {
        Config::settings.setValue("soundVolume", QVariant(soundVolume));
    }

    unsigned int getDailyPomodorosGoal() const override {
        return settings.value("dailyPomodorosGoal").toUInt();
    }

    void setDailyPomodorosGoal(unsigned dailyPomodorosGoal) override {
        Config::settings.setValue("dailyPomodorosGoal", QVariant(dailyPomodorosGoal));
    }

    unsigned int getWeeklyPomodorosGoal() const override {
        return settings.value("weeklyPomodorosGoal").toUInt();
    }

    void setWeeklyPomodorosGoal(unsigned weeklyPomodorosGoal) override {
        Config::settings.setValue("weeklyPomodorosGoal", QVariant(weeklyPomodorosGoal));
    }

    unsigned int getMonthlyPomodorosGoal() const override {
        return settings.value("monthlyPomodorosGoal").toUInt();
    }

    void setMonthlyPomodorosGoal(unsigned monthlyPomodorosGoal) override {
        Config::settings.setValue("monthlyPomodorosGoal", QVariant(monthlyPomodorosGoal));
    }

private:
    QSettings settings;
};

#endif //POMODORO_CONFIG_H
