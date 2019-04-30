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
#include "qt_gui/QtConfig.h"

namespace sprint_timer::ui::qt_gui {

std::chrono::minutes Config::sprintDuration() const
{
    return std::chrono::minutes{settings.value("sprintDuration", 25).toInt()};
}

void Config::setSprintDuration(std::chrono::minutes duration)
{
    Config::settings.setValue("sprintDuration",
                              QVariant(static_cast<int>(duration.count())));
}

std::chrono::minutes Config::shortBreakDuration() const
{
    return std::chrono::minutes{
        settings.value("shortBreakDuration", 5).toInt()};
}

void Config::setShortBreakDuration(std::chrono::minutes duration)
{
    Config::settings.setValue("shortBreakDuration",
                              QVariant(static_cast<int>(duration.count())));
}

std::chrono::minutes Config::longBreakDuration() const
{
    return std::chrono::minutes{
        settings.value("longBreakDuration", 15).toInt()};
}

void Config::setLongBreakDuration(std::chrono::minutes duration)
{
    Config::settings.setValue("longBreakDuration",
                              QVariant(static_cast<int>(duration.count())));
}

int Config::numSprintsBeforeBreak() const
{
    return settings.value("sprintsBeforeBreak", 4).toInt();
}

void Config::setNumSprintsBeforeBreak(int numSprints)
{
    Config::settings.setValue("sprintsBeforeBreak", QVariant(numSprints));
}

bool Config::soundIsEnabled() const
{
    return settings.value("playSound", true).toBool();
}

void Config::setPlaySound(bool playSound)
{
    Config::settings.setValue("playSound", QVariant(playSound));
}

int Config::soundVolume() const
{
    return settings.value("soundVolume", 50).toInt();
}

void Config::setSoundVolume(int soundVolume)
{
    Config::settings.setValue("soundVolume", QVariant(soundVolume));
}

std::string Config::soundFilePath() const
{
    return Config::settings.value("soundFilePath").toString().toStdString();
}

void Config::setSoundFilePath(const std::string& filePath)
{
    Config::settings.setValue("soundFilePath",
                              QVariant(QString::fromStdString(filePath)));
}

int Config::timerFlavour() const
{
    return Config::settings.value("timerFlavour", 1).toInt();
}

void Config::setTimerFlavour(int timerVariation)
{
    Config::settings.setValue("timerFlavour", QVariant(timerVariation));
}

utils::WeekdaySelection Config::workdays() const
{
    return utils::WeekdaySelection{
        Config::settings.value("workdaysCode", 127).toUInt()};
}

void Config::setWorkdays(const utils::WeekdaySelection& workdays)
{
    Config::settings.setValue("workdaysCode",
                              QVariant(workdays.selectionMask()));
}

dw::Weekday Config::firstDayOfWeek() const
{
    return static_cast<dw::Weekday>(
        Config::settings.value("firstDayOfWeek", 0).toInt());
}

void Config::setFirstDayOfWeek(dw::Weekday firstDayOfWeek)
{
    Config::settings.setValue("firstDayOfWeek",
                              QVariant(static_cast<int>(firstDayOfWeek)));
}

} // namespace sprint_timer::ui::qt_gui
