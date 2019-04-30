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
#ifndef QTSTORAGEINITIALIZER_H_WR5MUUAC
#define QTSTORAGEINITIALIZER_H_WR5MUUAC

#include <QCoreApplication>
#include <core/IConfig.h>
#include <qt_storage_impl/DBService.h>
#include <qt_storage_impl/QtStorageImplementersFactory.h>

/* Fixed configuration for testing purposes. */
class TestConfig : public sprint_timer::IConfig {

public:
    int soundVolume() const override { return mSoundVolume; }

    void setSoundVolume(int soundVolume) override
    {
        TestConfig::mSoundVolume = soundVolume;
    }

    std::chrono::minutes sprintDuration() const override
    {
        return mSprintDuration;
    }

    void setSprintDuration(std::chrono::minutes minutes) override
    {
        TestConfig::mSprintDuration = minutes;
    }

    std::chrono::minutes shortBreakDuration() const override
    {
        return mShortBreakDuration;
    }

    void setShortBreakDuration(std::chrono::minutes duration) override
    {
        TestConfig::mShortBreakDuration = duration;
    }

    std::chrono::minutes longBreakDuration() const override
    {
        return mLongBreakDuration;
    }

    void setLongBreakDuration(std::chrono::minutes duration) override
    {
        TestConfig::mLongBreakDuration = duration;
    }

    int numSprintsBeforeBreak() const override { return mTasksBeforeBreak; }

    void setNumSprintsBeforeBreak(int tasksBeforeBreak) override
    {
        TestConfig::mTasksBeforeBreak = tasksBeforeBreak;
    }

    bool soundIsEnabled() const override { return mPlaySound; }

    void setPlaySound(bool playSound) override
    {
        TestConfig::mPlaySound = playSound;
    }

    std::string soundFilePath() const override { return ""; }

    void setSoundFilePath(const std::string& filePath) override {}

    int timerFlavour() const override { return 0; }

    void setTimerFlavour(int timerVariation) override {}

    sprint_timer::utils::WeekdaySelection workdays() const override
    {
        return sprint_timer::utils::WeekdaySelection{31};
    }

    void
    setWorkdays(const sprint_timer::utils::WeekdaySelection& workdays) override
    {
    }

    dw::Weekday firstDayOfWeek() const override { return dw::Weekday::Monday; }

    void setFirstDayOfWeek(dw::Weekday firstDayOfWeek) override {}

private:
    std::chrono::minutes mSprintDuration{30};
    std::chrono::minutes mShortBreakDuration{10};
    std::chrono::minutes mLongBreakDuration{20};
    int mTasksBeforeBreak{4};
    bool mPlaySound{false};
    int mSoundVolume{0};
};


struct QtStorageInitializer {

    QtStorageInitializer();

    const QString name{"file::memory:?cache=shared"};
    QCoreApplication app;
    // QCoreApplication app{dummyArgc, &dummyArgv};
    sprint_timer::storage::qt_storage_impl::ConnectionGuard connectionGuard{
        name, "Keep alive conn"};
    sprint_timer::storage::qt_storage_impl::DBService dbService{name};
    TestConfig fakeSettings;
    sprint_timer::storage::qt_storage_impl::QtStorageImplementersFactory
        factory{dbService, fakeSettings};
    std::unique_ptr<sprint_timer::ITaskStorageReader> taskReader
        = factory.createTaskStorageReader();
    std::unique_ptr<sprint_timer::ITaskStorageWriter> taskWriter
        = factory.createTaskStorageWriter();
    std::unique_ptr<sprint_timer::ISprintStorageReader> sprintReader
        = factory.createSprintStorageReader();
    std::unique_ptr<sprint_timer::ISprintStorageWriter> sprintWriter
        = factory.createSprintStorageWriter();
    std::unique_ptr<sprint_timer::IYearRangeReader> yearRangeReader
        = factory.createYearRangeReader();
    std::unique_ptr<sprint_timer::ISprintDistributionReader>
        dailyDistributionReader = factory.createSprintDailyDistributionReader();
    std::unique_ptr<sprint_timer::ISprintDistributionReader>
        monthlyDistributionReader
        = factory.createSprintMonthlyDistributionReader();
    std::unique_ptr<sprint_timer::ISprintDistributionReader>
        mondayFirstWeeklyDistributionReader
        = factory.createSprintWeeklyDistributionReader(dw::Weekday::Monday);
    std::unique_ptr<sprint_timer::ISprintDistributionReader>
        sundayFirstWeeklyDistributionReader
        = factory.createSprintWeeklyDistributionReader(dw::Weekday::Sunday);
    std::unique_ptr<sprint_timer::IWorkingDaysWriter> workingDaysWriter
        = factory.createWorkingDaysWriter();
    std::unique_ptr<sprint_timer::IWorkingDaysReader> workingDaysReader
        = factory.createWorkingDaysReader();

    void runEventLoop();

    void quit();
};

#endif /* end of include guard: QTSTORAGEINITIALIZER_H_WR5MUUAC */
