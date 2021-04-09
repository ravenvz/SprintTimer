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

#include "core/CommandHandler.h"
#include "core/DataExporter.h"
#include "core/QueryHandler.h"
#include "core/use_cases/export_data/ExportTasksCommand.h"
#include "core/use_cases/request_tasks/FinishedTasksQuery.h"

namespace sprint_timer::use_cases {

class ExportTasksHandler : public CommandHandler<ExportTasksCommand> {
public:
    using Handler =
        QueryHandler<FinishedTasksQuery, std::vector<entities::Task>>;

    ExportTasksHandler(Handler& tasksHandler,
                       DataExporter<entities::Task>& exporter);

    void handle(ExportTasksCommand&& command) override;

private:
    Handler& tasksHandler;
    DataExporter<entities::Task>& exporter;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: EXPORTTASKSHANDLER_H_MSZJ51TI */

