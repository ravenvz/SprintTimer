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
#ifndef EXPORTSPRINTSHANDLER_H_EDGVZLLX
#define EXPORTSPRINTSHANDLER_H_EDGVZLLX

#include "core/CommandHandler.h"
#include "core/QueryHandler.h"
#include "core/use_cases/export_data/ExportSprintsCommand.h"
#include "core/use_cases/request_sprints/RequestSprintsQuery.h"

namespace sprint_timer::use_cases {

class ExportSprintsHandler : public CommandHandler<ExportSprintsCommand> {
public:
    using Handler = QueryHandler<RequestSprintsQuery>;

    ExportSprintsHandler(Handler& sprintsHandler,
                         DataExporter<entities::Sprint>& exporter);

    void handle(ExportSprintsCommand&& command) override;

private:
    Handler& sprintsHandler;
    DataExporter<entities::Sprint>& exporter;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: EXPORTSPRINTSHANDLER_H_EDGVZLLX */

