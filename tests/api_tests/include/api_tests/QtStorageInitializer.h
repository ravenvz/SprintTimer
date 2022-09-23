/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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

#include "api/DefaultDateTimeProvider.h"
#include "api/ObservableActionInvoker.h"
#include "api/dtos/NoteMapper.h"
#include "api/dtos/SprintMapper.h"
#include "api/dtos/TagMapper.h"
#include "api/dtos/TaskMapper.h"
#include "api/dtos/TaskTimeframeMapper.h"
#include "api/dtos/TaskTreeMapper.h"
#include "api/dtos/TaskTypeMapper.h"
#include "api_tests/FakeUuidGenerator.h"
#include "api_tests/TestCommandHandlerComposer.h"
#include "api_tests/TestQueryHandlerComposer.h"
#include "api_tests/constants.h"
#include "common_utils/DateTimeProviderMock.h"
#include "qt_storage/DatabaseInitializer.h"
#include "qt_storage/QtStorageImplementersFactory.h"
#include "qt_storage/WorkerConnection.h"
#include <QCoreApplication>

struct TestStorageInitializer {

    TestStorageInitializer();

    sprint_timer::compose::CommandHandlerComposer& commandHandlerComposer()
    {
        return *commandHandlerComp;
    }

    sprint_timer::compose::QueryHandlerComposer& queryHandlerComposer()
    {
        return *queryHandlerComp;
    }

    const sprint_timer::api::DateTimeProvider& dateTimeProvider() const
    {
        return dtProvider;
    }

private:
    QCoreApplication app;
    const QString name{"file::memory:?cache=shared"};
    sprint_timer::storage::qt_storage::ConnectionGuard connectionGuard{
        name, "Keep alive conn"};
    sprint_timer::storage::qt_storage::MigrationManager migrationManager;
    sprint_timer::storage::qt_storage::DatabaseInitializer db{name,
                                                              migrationManager};
    sprint_timer::storage::qt_storage::WorkerConnection dbService{
        name, "Worker connection"};
    sprint_timer::storage::qt_storage::QtStorageImplementersFactory factory{
        dbService.connectionName()};
    std::unique_ptr<sprint_timer::TaskStorage> taskStorage{
        factory.taskStorage()};
    std::unique_ptr<sprint_timer::SprintStorage> sprintStorage{
        factory.sprintStorage()};
    std::unique_ptr<sprint_timer::OperationalRangeReader>
        operationalRangeReader{factory.operationalRangeReader()};
    sprint_timer::ObservableActionInvoker actionInvoker;
    FakeUuidGenerator uuidGenerator;
    std::unique_ptr<sprint_timer::WorkScheduleStorage> workScheduleStorage{
        factory.scheduleStorage()};

    sprint_timer::api::NoteMapper noteMapper;
    sprint_timer::api::SprintDatetimeMapper sprintDateTimeMapper;
    sprint_timer::api::SprintMapper sprintMapper;
    sprint_timer::api::TagMapper tagMapper;
    sprint_timer::api::TaskTimeframeMapper timeFrameMapper;
    sprint_timer::api::TaskTypeMapper taskTypeMapper;
    sprint_timer::api::TaskMapper taskMapper{noteMapper,
                                             tagMapper,
                                             timeFrameMapper,
                                             taskTypeMapper,
                                             sprintDateTimeMapper};
    sprint_timer::api::TaskTreeMapper taskTreeMapper{taskMapper};

    std::unique_ptr<sprint_timer::compose::CommandHandlerComposer>
        commandHandlerComp{
            std::make_unique<sprint_timer::compose::TestCommandHandlerComposer>(
                actionInvoker,
                *taskStorage,
                *sprintStorage,
                *workScheduleStorage,
                uuidGenerator,
                dtProvider,
                taskMapper,
                sprintDateTimeMapper,
                taskTreeMapper)};
    std::unique_ptr<sprint_timer::SprintDistributionReader> dailyDistReader{
        factory.dailyDistReader(30)};
    std::unique_ptr<sprint_timer::SprintDistributionReader>
        mondayFirstDistReader{factory.weeklyDistReader(dw::Weekday::Monday)};
    std::unique_ptr<sprint_timer::SprintDistributionReader>
        sundayFirstDistReader{factory.weeklyDistReader(dw::Weekday::Sunday)};
    std::unique_ptr<sprint_timer::SprintDistributionReader> monthlyDistReader{
        factory.monthlyDistReader()};
    std::unique_ptr<sprint_timer::compose::QueryHandlerComposer>
        queryHandlerComp{
            std::make_unique<sprint_timer::compose::TestQueryHandlerComposer>(
                *taskStorage,
                *sprintStorage,
                *operationalRangeReader,
                *workScheduleStorage,
                *dailyDistReader,
                *mondayFirstDistReader,
                *sundayFirstDistReader,
                *monthlyDistReader,
                taskMapper,
                tagMapper,
                sprintMapper,
                taskTreeMapper)};
    sprint_timer::api::DefaultDateTimeProvider dtProvider;
};

#endif /* end of include guard: QTSTORAGEINITIALIZER_H_WR5MUUAC */
