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
#include "mocks/DataExporterMock.h"
#include "mocks/QueryHandlerMock.h"
#include <core/TaskBuilder.h>
#include <core/use_cases/export_data/ExportTasksHandler.h>
#include <core/use_cases/request_tasks/FinishedTasksQuery.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using namespace sprint_timer;

namespace sprint_timer::use_cases {

bool operator==(const FinishedTasksQuery& lhs, const FinishedTasksQuery& rhs)
{
    return lhs.dateRange == rhs.dateRange;
}

} // namespace sprint_timer::use_cases

class ExportTasksHandlerFixture : public ::testing::Test {
public:
    NiceMock<mocks::QueryHandlerMock<use_cases::FinishedTasksQuery,
                                     std::vector<entities::Task>>>
        requestTasksHandlerMock;
    NiceMock<mocks::DataExporterMock<entities::Task>> exporterMock;
    use_cases::ExportTasksHandler handler{requestTasksHandlerMock,
                                          exporterMock};
    const dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
};

TEST_F(ExportTasksHandlerFixture, delegates_to_exporter)
{
    TaskBuilder builder;
    builder.withUuid("123");
    const std::vector<entities::Task> tasks{builder.build(), builder.build()};
    ON_CALL(requestTasksHandlerMock,
            handle(use_cases::FinishedTasksQuery{someDateRange}))
        .WillByDefault(Return(tasks));

    EXPECT_CALL(exporterMock,
                exportData(tasks, DataFormat::Json, SinkType::Network));

    handler.handle(use_cases::ExportTasksCommand{
        someDateRange, DataFormat::Json, SinkType::Network});
}
