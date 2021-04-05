/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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

// TODO don't think this belongs to the core. Move it from the core when jobs to
// purge depencies on settings are done application-wise

#include <chrono>
#include <date_wrapper/date_wrapper.h>
#include <string>

namespace sprint_timer {

class IConfig {
public:
    virtual ~IConfig() = default;

    virtual std::chrono::minutes sprintDuration() const = 0;

    virtual void setSprintDuration(std::chrono::minutes duration) = 0;

    virtual std::chrono::minutes shortBreakDuration() const = 0;

    virtual void setShortBreakDuration(std::chrono::minutes duration) = 0;

    virtual std::chrono::minutes longBreakDuration() const = 0;

    virtual void setLongBreakDuration(std::chrono::minutes duration) = 0;

    virtual int numSprintsBeforeBreak() const = 0;

    virtual void setNumSprintsBeforeBreak(int tasksBeforeBreak) = 0;

    virtual bool soundIsEnabled() const = 0;

    virtual void setPlaySound(bool playSound) = 0;

    virtual int soundVolume() const = 0;

    // TODO split into separate group
    virtual void setSoundVolume(int soundVolume) = 0;

    virtual std::string soundFilePath() const = 0;

    virtual void setSoundFilePath(const std::string& filePath) = 0;

    virtual int timerFlavour() const = 0;

    virtual void setTimerFlavour(int timerVariation) = 0;

    virtual dw::Weekday firstDayOfWeek() const = 0;

    virtual void setFirstDayOfWeek(dw::Weekday firstDayOfWeek) = 0;
};

} // namespace sprint_timer

#endif // ICONFIG_H
