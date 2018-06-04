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
#ifndef QT_CONFIG_H_3HPQW7BI
#define QT_CONFIG_H_3HPQW7BI

#include "core/IConfig.h"
#include <QSettings>
#include <QVariant>

/* Implements IConfig application config interface using Qt framework
 * features. */
class Config : public IConfig {

public:
    int sprintDuration() const override
    {
        return settings.value("sprintDuration", 25).toInt();
    }

    void setSprintDuration(int minutes) override
    {
        Config::settings.setValue("sprintDuration",
                                  QVariant(minutes));
    }

    int shortBreakDuration() const override
    {
        return settings.value("shortBreakDuration", 5).toInt();
    }

    void setShortBreakDuration(int minutes) override
    {
        Config::settings.setValue("shortBreakDuration",
                                  QVariant(minutes));
    }

    int longBreakDuration() const override
    {
        return settings.value("longBreakDuration", 15).toInt();
    }

    void setLongBreakDuration(int minutes) override
    {
        Config::settings.setValue("longBreakDuration",
                                  QVariant(minutes));
    }

    int numSprintsBeforeBreak() const override
    {
        return settings.value("sprintsBeforeBreak", 4).toInt();
    }

    void setNumSprintsBeforeBreak(int numSprints) override
    {
        Config::settings.setValue("sprintsBeforeBreak",
                                  QVariant(numSprints));
    }

    bool soundIsEnabled() const override
    {
        return settings.value("playSound", true).toBool();
    }

    void setPlaySound(bool playSound) override
    {
        Config::settings.setValue("playSound", QVariant(playSound));
    }

    int soundVolume() const override
    {
        return settings.value("soundVolume", 50).toInt();
    }

    void setSoundVolume(int soundVolume) override
    {
        Config::settings.setValue("soundVolume", QVariant(soundVolume));
    }

    int dailyGoal() const override
    {
        return settings.value("dailyGoal", 11).toInt();
    }

    void setDailyGoal(int numSprints) override
    {
        Config::settings.setValue("dailyGoal",
                                  QVariant(numSprints));
    }

    int weeklyGoal() const override
    {
        return settings.value("weeklyGoal", 65).toInt();
    }

    void setWeeklyGoal(int numSprints) override
    {
        Config::settings.setValue("weeklyGoal",
                                  QVariant(numSprints));
    }

    int monthlyGoal() const override
    {
        return settings.value("monthlyGoal", 260).toInt();
    }

    void setMonthlyGoal(int numSprints) override
    {
        Config::settings.setValue("monthlyGoal",
                                  QVariant(numSprints));
    }

    std::string soundFilePath() const override
    {
        return Config::settings.value("soundFilePath").toString().toStdString();
    }

    void setSoundFilePath(const std::string& filePath) override
    {
        Config::settings.setValue("soundFilePath",
                                  QVariant(QString::fromStdString(filePath)));
    }

    int timerFlavour() const override
    {
        return Config::settings.value("timerFlavour", 1).toInt();
    }

    void setTimerFlavour(int timerVariation) override
    {
        Config::settings.setValue("timerFlavour", QVariant(timerVariation));
    }

    unsigned workdaysCode() const override {
        return Config::settings.value("workdaysCode", 127).toUInt();
    }

    void setWorkdaysCode(unsigned workdays_code) override {
        Config::settings.setValue("workdaysCode", QVariant(workdays_code));
    }

private:
    QSettings settings;
};

#endif /* end of include guard: QT_CONFIG_H_3HPQW7BI */
