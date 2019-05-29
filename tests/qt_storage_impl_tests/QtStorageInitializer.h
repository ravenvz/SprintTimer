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
#include <qt_storage_impl/DBService.h>
#include <qt_storage_impl/QtStorageImplementersFactory.h>


struct QtStorageInitializer {

    QtStorageInitializer();

    const QString name{"file::memory:?cache=shared"};
    QCoreApplication app;
    sprint_timer::storage::qt_storage_impl::ConnectionGuard connectionGuard{
        name, "Keep alive conn"};
    sprint_timer::storage::qt_storage_impl::DBService dbService{name};
    sprint_timer::storage::qt_storage_impl::QtStorageImplementersFactory
        factory{dbService};
    std::unique_ptr<sprint_timer::ITaskStorage> taskStorage
        = factory.createTaskStorage();
    std::unique_ptr<sprint_timer::ISprintStorage> sprintStorage
        = factory.createSprintStorage();
    std::unique_ptr<sprint_timer::IOperationalRangeReader>
        operationalRangeReader = factory.createOperationalRangeReader();
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
    std::unique_ptr<sprint_timer::IWorkingDaysStorage> workingDaysStorage
        = factory.createWorkingDaysStorage();

    void runEventLoop();

    void quit();
};

#endif /* end of include guard: QTSTORAGEINITIALIZER_H_WR5MUUAC */
