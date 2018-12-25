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
#ifndef QTSTORAGEINITIALIZER_H_WR5MUUAC
#define QTSTORAGEINITIALIZER_H_WR5MUUAC

#include <QCoreApplication>
#include <qt_storage_impl/DBService.h>
#include <qt_storage_impl/QtStorageImplementersFactory.h>

struct QtStorageInitializer {

	QtStorageInitializer();

	const QString name{":memory:"};
    QCoreApplication app{dummyArgc, &dummyArgv};
	sprint_timer::storage::qt_storage_impl::ConnectionGuard connectionGuard{name, "Keep alive conn"};
    sprint_timer::storage::qt_storage_impl::DBService dbService{name};
    sprint_timer::storage::qt_storage_impl::QtStorageImplementersFactory
        factory{dbService};
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
        = factory.createSprintWeeklyDistributionReader(
            sprint_timer::FirstDayOfWeek::Monday);
    std::unique_ptr<sprint_timer::ISprintDistributionReader>
        sundayFirstWeeklyDistributionReader
        = factory.createSprintWeeklyDistributionReader(
            sprint_timer::FirstDayOfWeek::Sunday);

    void runEventLoop();

    void quit();

private:
    int dummyArgc{0};
    char* dummyArgv{nullptr};
};

#endif /* end of include guard: QTSTORAGEINITIALIZER_H_WR5MUUAC */
