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
#ifndef QT_CONFIG_H_3HPQW7BI
#define QT_CONFIG_H_3HPQW7BI

#include "core/IConfig.h"
#include <QSettings>
#include <QVariant>

/* Implements IConfig application config interface using Qt framework
 * features. */
class Config : public IConfig {

public:
    int pomodoroDuration() const final
    {
        return settings.value("pomodoroDuration").toInt();
    }

    void setPomodoroDuration(int pomodoroDuration) final
    {
        Config::settings.setValue("pomodoroDuration",
                                  QVariant(pomodoroDuration));
    }

    int shortBreakDuration() const final
    {
        return settings.value("shortBreakDuration").toInt();
    }

    void setShortBreakDuration(int shortBreakDuration) final
    {
        Config::settings.setValue("shortBreakDuration",
                                  QVariant(shortBreakDuration));
    }

    int longBreakDuration() const final
    {
        return settings.value("longBreakDuration").toInt();
    }

    void setLongBreakDuration(int longBreakDuration) final
    {
        Config::settings.setValue("longBreakDuration",
                                  QVariant(longBreakDuration));
    }

    int numPomodorosBeforeBreak() const final
    {
        return settings.value("tasksBeforeBreak").toInt();
    }

    void setPomodorosBeforeBreak(int tasksBeforeBreak) final
    {
        Config::settings.setValue("tasksBeforeBreak",
                                  QVariant(tasksBeforeBreak));
    }

    bool soundIsEnabled() const final
    {
        return settings.value("playSound").toBool();
    }

    void setPlaySound(bool playSound) final
    {
        Config::settings.setValue("playSound", QVariant(playSound));
    }

    int soundVolume() const final
    {
        return settings.value("soundVolume").toInt();
    }

    void setSoundVolume(int soundVolume) final
    {
        Config::settings.setValue("soundVolume", QVariant(soundVolume));
    }

    int dailyPomodorosGoal() const final
    {
        return settings.value("dailyPomodorosGoal").toInt();
    }

    void setDailyPomodorosGoal(int dailyPomodorosGoal) final
    {
        Config::settings.setValue("dailyPomodorosGoal",
                                  QVariant(dailyPomodorosGoal));
    }

    int weeklyPomodorosGoal() const final
    {
        return settings.value("weeklyPomodorosGoal").toInt();
    }

    void setWeeklyPomodorosGoal(int weeklyPomodorosGoal) final
    {
        Config::settings.setValue("weeklyPomodorosGoal",
                                  QVariant(weeklyPomodorosGoal));
    }

    int monthlyPomodorosGoal() const final
    {
        return settings.value("monthlyPomodorosGoal").toInt();
    }

    void setMonthlyPomodorosGoal(int monthlyPomodorosGoal) final
    {
        Config::settings.setValue("monthlyPomodorosGoal",
                                  QVariant(monthlyPomodorosGoal));
    }

    std::string soundFilePath() const final
    {
        return Config::settings.value("soundFilePath").toString().toStdString();
    }

    void setSoundFilePath(const std::string& filePath) final
    {
        Config::settings.setValue("soundFilePath",
                                  QVariant(QString::fromStdString(filePath)));
    }

    int timerFlavour() const final
    {
        return Config::settings.value("timerFlavour").toInt();
    }

    void setTimerFlavour(int timerVariation)
    {
        Config::settings.setValue("timerFlavour", QVariant(timerVariation));
    }

private:
    QSettings settings;
};

#endif /* end of include guard: QT_CONFIG_H_3HPQW7BI */
