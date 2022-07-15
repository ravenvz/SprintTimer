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
#ifndef EXPORTTASKSHANDLER_H_MSZJ51TI
#define EXPORTTASKSHANDLER_H_MSZJ51TI

#include "api/com_query/CommandHandler.h"
#include "api/com_query/QueryHandler.h"
#include "api/requests/ExportTasksCommand.h"
#include "api/requests/FinishedTasksQuery.h"
#include "core/DataExporter.h"

namespace sprint_timer::api {

class ExportTasksHandler : public asp::CommandHandler<ExportTasksCommand> {
public:
    using finished_tasks_handler_t = asp::QueryHandler<FinishedTasksQuery>;

    ExportTasksHandler(finished_tasks_handler_t& tasksHandler,
                       DataExporter<TaskDTO>& exporter);

    void handle(const ExportTasksCommand& command) override;

private:
    finished_tasks_handler_t& tasksHandler;
    DataExporter<TaskDTO>& exporter;
};

} // namespace sprint_timer::api

#endif /* end of include guard: EXPORTTASKSHANDLER_H_MSZJ51TI */

