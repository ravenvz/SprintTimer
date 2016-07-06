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
#ifndef POMODORO_CONFIG_H
#define POMODORO_CONFIG_H

#include <string>

class IConfig {

public:
    virtual ~IConfig() = default;
    virtual int pomodoroDuration() const = 0;
    virtual void setPomodoroDuration(int pomodoroDuration) = 0;
    virtual int shortBreakDuration() const = 0;
    virtual void setShortBreakDuration(int shortBreakDuration) = 0;
    virtual int longBreakDuration() const = 0;
    virtual void setLongBreakDuration(int longBreakDuration) = 0;
    virtual int numPomodorosBeforeBreak() const = 0;
    virtual void setPomodorosBeforeBreak(int tasksBeforeBreak) = 0;
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
    virtual std::string soundFilePath() = 0;
    virtual void setSoundFilePath(const std::string& filePath) = 0;
};


#endif // POMODORO_CONFIG_H
