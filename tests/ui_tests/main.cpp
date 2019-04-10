/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#include "QtConfig.h"
#include "TestUserScenarios.h"
#include "TestCase.h"
#include "qt_storage_impl/DBService.h"
#include "qt_storage_impl/QtStorageImplementersFactory.h"
#include <core/CoreService.h>


int main(int argc, char* argv[])
{
    QApplication app{argc, argv};
    DBService dbService{QString{"sprint_test.db"}};

    QtStorageImplementersFactory factory{dbService};
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QApplication::setOrganizationName("RavenStudio");
    QApplication::setApplicationName("TestSprintTimer");
    Config applicationSettings;

    std::unique_ptr<ISprintStorageReader> sprintStorageReader{
        factory.createSprintStorageReader()};
    std::unique_ptr<ISprintStorageWriter> sprintStorageWriter{
        factory.createSprintStorageWriter()};
    std::unique_ptr<IYearRangeReader> sprintYearRangeReader{
        factory.createYearRangeReader()};
    std::unique_ptr<ITaskStorageReader> taskStorageReader{
        factory.createTaskStorageReader()};
    std::unique_ptr<ITaskStorageWriter> taskStorageWriter{
        factory.createTaskStorageWriter()};
    std::unique_ptr<ISprintDistributionReader> dailyDistributionReader{
        factory.createSprintDailyDistributionReader()};
    std::unique_ptr<ISprintDistributionReader> weeklyDistributionReader{
        factory.createSprintWeeklyDistributionReader()};
    std::unique_ptr<ISprintDistributionReader> monthlyDistributionReader{
        factory.createSprintMonthlyDistributionReader()};

    Core::CoreService coreService{*sprintStorageReader.get(),
                                  *sprintStorageWriter.get(),
                                  *sprintYearRangeReader.get(),
                                  *taskStorageReader.get(),
                                  *taskStorageWriter.get(),
                                  *dailyDistributionReader.get(),
                                  *weeklyDistributionReader.get(),
                                  *monthlyDistributionReader.get()};

    MainWindow mainWindow{applicationSettings, coreService};
    mainWindow.show();

    TestUserScenarios testAddTask{&app, &mainWindow};
    QTest::qExec(&testAddTask, argc, argv);

    return 0;
}
