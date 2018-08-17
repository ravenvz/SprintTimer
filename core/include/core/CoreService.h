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
#ifndef CORESERVICE_H_JXK8PKAI
#define CORESERVICE_H_JXK8PKAI


#include "core/CommandInvoker.h"
#include "core/ICoreService.h"
#include "core/ISprintDistributionReader.h"
#include "core/ISprintStorageReader.h"
#include "core/ISprintStorageWriter.h"
#include "core/ITaskStorageReader.h"
#include "core/ITaskStorageWriter.h"
#include "core/IYearRangeReader.h"
#include "core/MacroTransaction.h"
#include "core/QueryExecutor.h"

namespace sprint_timer {

class CoreService : public ICoreService {
public:
    CoreService(ISprintStorageReader& sprintStorageReader,
                ISprintStorageWriter& sprintStorageWriter,
                ITaskStorageReader& taskStorageReader,
                ITaskStorageWriter& taskStorageWriter,
                CommandInvoker& invoker,
                QueryExecutor& queryExecutor);

    void exportTasks(const dw::TimeSpan& timeSpan,
                     std::shared_ptr<external_io::ISink> sink,
                     TaskEncodingFunc func) final;

    void registerSprint(const dw::TimeSpan& timeSpan,
                        const std::string& taskUuid) final;

    void registerSprint(const entities::Sprint& sprint) final;

    void removeSprint(const entities::Sprint& sprint) final;

    void exportSprints(const dw::TimeSpan& timeSpan,
                       std::shared_ptr<external_io::ISink> sink,
                       SprintEncodingFunc func) final;

    std::string lastCommandDescription() const final;

    uint64_t numRevertableCommands() const final;

    void undoLast() final;

    void registerUndoObserver(Observer& observer) final;

private:
    ISprintStorageReader& sprintReader;
    ISprintStorageWriter& sprintWriter;
    ITaskStorageReader& taskReader;
    ITaskStorageWriter& taskWriter;
    CommandInvoker& invoker;
    QueryExecutor& query_invoker;
};

} // namespace sprint_timer

#endif /* end of include guard: CORESERVICE_H_JXK8PKAI */
