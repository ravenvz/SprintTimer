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
** but WITHOUT ANY WARRANTY without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "ObservableConfig.h"

namespace sprint_timer::compose {

ObservableConfig::ObservableConfig(IConfig& wrapped_)
    : wrapped{wrapped_}
{
}

std::chrono::minutes ObservableConfig::sprintDuration() const
{
    return wrapped.sprintDuration();
}

void ObservableConfig::setSprintDuration(std::chrono::minutes duration)
{
    wrapped.setSprintDuration(duration);
    notify();
}

std::chrono::minutes ObservableConfig::shortBreakDuration() const
{
    return wrapped.shortBreakDuration();
}

void ObservableConfig::setShortBreakDuration(std::chrono::minutes duration)
{
    wrapped.setShortBreakDuration(duration);
    notify();
}

std::chrono::minutes ObservableConfig::longBreakDuration() const
{
    return wrapped.longBreakDuration();
}

void ObservableConfig::setLongBreakDuration(std::chrono::minutes duration)
{
    wrapped.setLongBreakDuration(duration);
    notify();
}

int ObservableConfig::numSprintsBeforeBreak() const
{
    return wrapped.numSprintsBeforeBreak();
}

void ObservableConfig::setNumSprintsBeforeBreak(int numSprints)
{
    wrapped.setNumSprintsBeforeBreak(numSprints);
    notify();
}

bool ObservableConfig::soundIsEnabled() const
{
    return wrapped.soundIsEnabled();
}

void ObservableConfig::setPlaySound(bool playSound)
{
    wrapped.setPlaySound(playSound);
    notify();
}

int ObservableConfig::soundVolume() const { return wrapped.soundVolume(); }

void ObservableConfig::setSoundVolume(int soundVolume)
{
    wrapped.setSoundVolume(soundVolume);
    notify();
}

std::string ObservableConfig::soundFilePath() const
{
    return wrapped.soundFilePath();
}

void ObservableConfig::setSoundFilePath(const std::string& filePath)
{
    wrapped.setSoundFilePath(filePath);
    notify();
}

int ObservableConfig::timerFlavour() const { return wrapped.timerFlavour(); }

void ObservableConfig::setTimerFlavour(int timerVariation)
{
    wrapped.setTimerFlavour(timerVariation);
    notify();
}

dw::Weekday ObservableConfig::firstDayOfWeek() const
{
    return wrapped.firstDayOfWeek();
}

void ObservableConfig::setFirstDayOfWeek(dw::Weekday firstDayOfWeek)
{
    wrapped.setFirstDayOfWeek(firstDayOfWeek);
    notify();
}

} // namespace sprint_timer::compose

