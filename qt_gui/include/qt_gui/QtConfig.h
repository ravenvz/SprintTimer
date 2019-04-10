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
#ifndef QT_CONFIG_H_3HPQW7BI
#define QT_CONFIG_H_3HPQW7BI

#include <QSettings>
#include <QVariant>
#include <core/IConfig.h>

namespace sprint_timer::ui::qt_gui {

/* Implements IConfig application config interface using Qt framework
 * features. */
class Config : public IConfig {

public:
    std::chrono::minutes sprintDuration() const override;

    void setSprintDuration(std::chrono::minutes duration) override;

    std::chrono::minutes shortBreakDuration() const override;

    void setShortBreakDuration(std::chrono::minutes duration) override;

    std::chrono::minutes longBreakDuration() const override;

    void setLongBreakDuration(std::chrono::minutes duration) override;

    int numSprintsBeforeBreak() const override;

    void setNumSprintsBeforeBreak(int numSprints) override;

    bool soundIsEnabled() const override;

    void setPlaySound(bool playSound) override;

    int soundVolume() const override;

    void setSoundVolume(int soundVolume) override;

    int dailyGoal() const override;

    void setDailyGoal(int numSprints) override;

    int weeklyGoal() const override;

    void setWeeklyGoal(int numSprints) override;

    int monthlyGoal() const override;

    void setMonthlyGoal(int numSprints) override;

    std::string soundFilePath() const override;

    void setSoundFilePath(const std::string& filePath) override;

    int timerFlavour() const override;

    void setTimerFlavour(int timerVariation) override;

    utils::WeekdaySelection workdays() const override;

    void setWorkdays(const utils::WeekdaySelection& workdays) override;

    FirstDayOfWeek firstDayOfWeek() const override;

    void setFirstDayOfWeek(FirstDayOfWeek firstDayOfWeek) override;

private:
    QSettings settings;
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: QT_CONFIG_H_3HPQW7BI */
