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
#include "core/use_cases/export_data/ExportTasksHandler.h"

namespace sprint_timer::use_cases {

ExportTasksHandler::ExportTasksHandler(Handler& tasksHandler_,

                                       DataExporter<entities::Task>& exporter_)
    : tasksHandler{tasksHandler_}
    , exporter{exporter_}
{
}

void ExportTasksHandler::handle(ExportTasksCommand&& command)
{
    const auto tasks =
        tasksHandler.handle(FinishedTasksQuery{command.dateRange});
    exporter.exportData(tasks, command.format, command.sinkType);
}

} // namespace sprint_timer::use_cases
