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

#include "core/ObservableActionInvoker.h"
#include "qt_storage/DatabaseInitializer.h"
#include "qt_storage/QtStorageImplementersFactory.h"
#include "qt_storage/WorkerConnection.h"
#include "use_cases_tests/TestCommandHandlerComposer.h"
#include "use_cases_tests/TestQueryHandlerComposer.h"
#include <QCoreApplication>

struct TestStorageInitializer {

    TestStorageInitializer();

    auto& commandHandlerComposer() { return commandHandlerComp; }

    auto& queryHandlerComposer() { return queryHandlerComp; }

private:
    QCoreApplication app;
    const QString name{"file::memory:?cache=shared"};
    sprint_timer::storage::qt_storage::ConnectionGuard connectionGuard{
        name, "Keep alive conn"};
    sprint_timer::storage::qt_storage::DatabaseInitializer db{name};
    sprint_timer::storage::qt_storage::WorkerConnection dbService{
        name, "Worker connection"};
    sprint_timer::storage::qt_storage::QtStorageImplementersFactory factory{
        dbService.connectionName()};
    std::unique_ptr<sprint_timer::TaskStorage> taskStorage{
        factory.taskStorage()};
    std::unique_ptr<sprint_timer::SprintStorage> sprintStorage{
        factory.sprintStorage()};
    sprint_timer::ObservableActionInvoker actionInvoker;
    sprint_timer::compose::TestCommandHandlerComposer commandHandlerComp{
        actionInvoker, *taskStorage, *sprintStorage};
    sprint_timer::compose::TestQueryHandlerComposer queryHandlerComp{
        *taskStorage, *sprintStorage};
};

#endif /* end of include guard: QTSTORAGEINITIALIZER_H_WR5MUUAC */
