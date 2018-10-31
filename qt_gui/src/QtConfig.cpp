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

int Config::dailyGoal() const
{
    return settings.value("dailyGoal", 11).toInt();
}

void Config::setDailyGoal(int numSprints)
{
    Config::settings.setValue("dailyGoal", QVariant(numSprints));
}

int Config::weeklyGoal() const
{
    return settings.value("weeklyGoal", 65).toInt();
}

void Config::setWeeklyGoal(int numSprints)
{
    Config::settings.setValue("weeklyGoal", QVariant(numSprints));
}

int Config::monthlyGoal() const
{
    return settings.value("monthlyGoal", 260).toInt();
}

void Config::setMonthlyGoal(int numSprints)
{
    Config::settings.setValue("monthlyGoal", QVariant(numSprints));
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

unsigned Config::workdaysCode() const
{
    return Config::settings.value("workdaysCode", 127).toUInt();
}

void Config::setWorkdaysCode(unsigned workdays_code)
{
    Config::settings.setValue("workdaysCode", QVariant(workdays_code));
}

} // namespace sprint_timer::ui::qt_gui
