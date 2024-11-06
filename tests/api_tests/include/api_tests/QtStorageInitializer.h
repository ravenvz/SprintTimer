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

#include "api/ActionInvokerImpl.h"
#include "api/DefaultDateTimeProvider.h"
#include "api/ObservableActionInvoker.h"
#include "api/SprintTimerAdapter.h"
#include "api/dtos/NoteMapper.h"
#include "api/dtos/SprintMapper.h"
#include "api/dtos/TagMapper.h"
#include "api/dtos/TaskMapper.h"
#include "api/dtos/TaskTimeframeMapper.h"
#include "api/dtos/TaskTreeMapper.h"
#include "api/dtos/TaskTypeMapper.h"
#include "api_tests/FakeUuidGenerator.h"
#include "api_tests/TestRequestHandlerComposer.h"
#include "api_tests/constants.h"
#include "common_utils/ConfigurableDateTimeProvider.h"
#include "common_utils/DateTimeProviderMock.h"
#include "core/ComputeByDayStrategy.h"
#include "core/RequestForDaysBack.h"
#include "core/SprintTimer.h"
#include "qt_storage/DatabaseInitializer.h"
#include "qt_storage/QtStorageImplementersFactory.h"
#include "qt_storage/WorkerConnection.h"
#include <QCoreApplication>

struct TestStorageInitializer {
    // Creates initializer with default date time provider
    TestStorageInitializer();

    // Creates initializer with fixed timepoint date time provider
    TestStorageInitializer(dw::DateTime fixedTimepoint);

    auto
    requestHandlerComposer() -> sprint_timer::compose::RequestHandlerComposer&
    {
        return *handlerComposer;
    }

    sprint_timer::compose::CommandHandlerComposer& commandHandlerComposer()
    {
        return *handlerComposer;
    }

    sprint_timer::compose::QueryHandlerComposer& queryHandlerComposer()
    {
        return *handlerComposer;
    }

    auto getDateTimeProvider() -> ConfigurableDateTimeProvider&
    {
        return *dtProvider;
    }

    auto installWorkflowListener(
        sprint_timer::api::TimerWorkflowListener* listener) -> void
    {
        sprintTimerAdapter.addListener(listener);
    }

    auto removeWorkflowListener(
        sprint_timer::api::TimerWorkflowListener* listener) -> void
    {
        sprintTimerAdapter.removeListener(listener);
    }

private:
    QCoreApplication app;
    std::unique_ptr<ConfigurableDateTimeProvider> dtProvider;
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
    std::unique_ptr<sprint_timer::api::TaskStorage> taskStorage{
        factory.taskStorage()};
    std::unique_ptr<sprint_timer::api::SprintStorage> sprintStorage{
        factory.sprintStorage()};
    std::unique_ptr<sprint_timer::api::OperationalRangeReader>
        operationalRangeReader{factory.operationalRangeReader(*dtProvider)};
    sprint_timer::ActionInvokerImpl defaultActionInvoker;
    sprint_timer::ObservableActionInvoker actionInvoker{defaultActionInvoker};
    FakeUuidGenerator uuidGenerator;
    std::unique_ptr<sprint_timer::api::WorkScheduleStorage> workScheduleStorage{
        factory.scheduleStorage()};
    sprint_timer::RequestForDaysBack requestDaysBack{30};
    sprint_timer::ComputeByDayStrategy computeByDay;
    sprint_timer::BackgroundCountdownTimer timer;
    sprint_timer::SprintTimer sprintTimer{
        timer,
        std::chrono::seconds{1},
        sprint_timer::SprintTimer::WorkflowParams{std::chrono::seconds{25 * 60},
                                                  std::chrono::seconds{5 * 60},
                                                  std::chrono::seconds{15 * 60},
                                                  4}};
    sprint_timer::api::SprintTimerAdapter sprintTimerAdapter{sprintTimer};

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
    std::unique_ptr<sprint_timer::api::SprintDistributionReader>
        dailyDistReader{factory.dailyDistReader(30)};
    std::unique_ptr<sprint_timer::api::SprintDistributionReader>
        mondayFirstDistReader{factory.weeklyDistReader(dw::Weekday::Monday)};
    std::unique_ptr<sprint_timer::api::SprintDistributionReader>
        sundayFirstDistReader{factory.weeklyDistReader(dw::Weekday::Sunday)};
    std::unique_ptr<sprint_timer::api::SprintDistributionReader>
        monthlyDistReader{factory.monthlyDistReader()};

    std::unique_ptr<sprint_timer::compose::RequestHandlerComposer>
        handlerComposer{
            std::make_unique<sprint_timer::compose::TestRequestHandlerComposer>(
                *taskStorage,
                *sprintStorage,
                *operationalRangeReader,
                *workScheduleStorage,
                *dailyDistReader,
                *mondayFirstDistReader,
                *sundayFirstDistReader,
                *monthlyDistReader,
                requestDaysBack,
                computeByDay,
                taskMapper,
                tagMapper,
                sprintMapper,
                taskTreeMapper,
                actionInvoker,
                uuidGenerator,
                *dtProvider,
                sprintTimer)};
};

#endif /* end of include guard: QTSTORAGEINITIALIZER_H_WR5MUUAC */
