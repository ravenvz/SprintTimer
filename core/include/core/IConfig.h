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
#ifndef ICONFIG_H
#define ICONFIG_H

#include <string>

class IConfig {

public:
    virtual ~IConfig() = default;
    virtual int sprintDuration() const = 0;
    virtual void setSprintDuration(int minutes) = 0;
    virtual int shortBreakDuration() const = 0;
    virtual void setShortBreakDuration(int minutes) = 0;
    virtual int longBreakDuration() const = 0;
    virtual void setLongBreakDuration(int minutes) = 0;
    virtual int numSprintsBeforeBreak() const = 0;
    virtual void setNumSprintsBeforeBreak(int tasksBeforeBreak) = 0;
    virtual bool soundIsEnabled() const = 0;
    virtual void setPlaySound(bool playSound) = 0;
    virtual int soundVolume() const = 0;
    // TODO split into separate group
    virtual void setSoundVolume(int soundVolume) = 0;
    virtual int dailyGoal() const = 0;
    virtual void setDailyGoal(int numSprints) = 0;
    virtual int weeklyGoal() const = 0;
    virtual void setWeeklyGoal(int numSprints) = 0;
    virtual int monthlyGoal() const = 0;
    virtual void setMonthlyGoal(int numSprints) = 0;
    virtual std::string soundFilePath() const = 0;
    virtual void setSoundFilePath(const std::string& filePath) = 0;
    virtual int timerFlavour() const = 0;
    virtual void setTimerFlavour(int timerVariation) = 0;
    virtual unsigned workdaysCode() const = 0;
    virtual void setWorkdaysCode(unsigned workdays_code) = 0;
};


#endif // ICONFIG_H
