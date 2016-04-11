#ifndef QT_CONFIG_H_3HPQW7BI
#define QT_CONFIG_H_3HPQW7BI

#include "core/config.h"

/* Implements IConfig application config interface using Qt framework
 * features. */
class Config : public IConfig {

public:
    int pomodoroDuration() const override
    {
        return settings.value("pomodoroDuration").toInt();
    }

    void setPomodoroDuration(int pomodoroDuration) override
    {
        Config::settings.setValue("pomodoroDuration",
                                  QVariant(pomodoroDuration));
    }

    int shortBreakDuration() const override
    {
        return settings.value("shortBreakDuration").toInt();
    }

    void setShortBreakDuration(int shortBreakDuration) override
    {
        Config::settings.setValue("shortBreakDuration",
                                  QVariant(shortBreakDuration));
    }

    int longBreakDuration() const override
    {
        return settings.value("longBreakDuration").toInt();
    }

    void setLongBreakDuration(int longBreakDuration) override
    {
        Config::settings.setValue("longBreakDuration",
                                  QVariant(longBreakDuration));
    }

    int numPomodorosBeforeBreak() const override
    {
        return settings.value("tasksBeforeBreak").toInt();
    }

    void setPomodorosBeforeBreak(int tasksBeforeBreak) override
    {
        Config::settings.setValue("tasksBeforeBreak",
                                  QVariant(tasksBeforeBreak));
    }

    bool soundIsEnabled() const override
    {
        return settings.value("playSound").toBool();
    }

    void setPlaySound(bool playSound) override
    {
        Config::settings.setValue("playSound", QVariant(playSound));
    }

    int soundVolume() const override
    {
        return settings.value("soundVolume").toInt();
    }

    void setSoundVolume(int soundVolume) override
    {
        Config::settings.setValue("soundVolume", QVariant(soundVolume));
    }

    int dailyPomodorosGoal() const override
    {
        return settings.value("dailyPomodorosGoal").toInt();
    }

    void setDailyPomodorosGoal(int dailyPomodorosGoal) override
    {
        Config::settings.setValue("dailyPomodorosGoal",
                                  QVariant(dailyPomodorosGoal));
    }

    int weeklyPomodorosGoal() const override
    {
        return settings.value("weeklyPomodorosGoal").toInt();
    }

    void setWeeklyPomodorosGoal(int weeklyPomodorosGoal) override
    {
        Config::settings.setValue("weeklyPomodorosGoal",
                                  QVariant(weeklyPomodorosGoal));
    }

    int monthlyPomodorosGoal() const override
    {
        return settings.value("monthlyPomodorosGoal").toInt();
    }

    void setMonthlyPomodorosGoal(int monthlyPomodorosGoal) override
    {
        Config::settings.setValue("monthlyPomodorosGoal",
                                  QVariant(monthlyPomodorosGoal));
    }

private:
    QSettings settings;
};

#endif /* end of include guard: QT_CONFIG_H_3HPQW7BI */
