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
#include "api/handlers/ExportTasksHandler.h"
#include "api/requests/FinishedTasksQuery.h"
#include <gmock/gmock.h>
// #include "mocks/DataExporterMock.h"
// #include "mocks/QueryHandlerMock.h"

// using api::TaskDTO;
// using ::testing::_;
// using ::testing::NiceMock;
// using ::testing::Return;
// using namespace sprint_timer;
// using namespace dw;

// TODO make this an integration test or move to external_io tests

class DISABLED_ExportTasksHandlerFixture : public ::testing::Test {
public:
    // NiceMock<mocks::QueryHandlerMock<api::FinishedTasksQuery>>
    //     requestTasksHandlerMock;
    // NiceMock<mocks::DataExporterMock<api::TaskDTO>> exporterMock;
    // api::ExportTasksHandler handler{requestTasksHandlerMock, exporterMock};
    // const dw::DateRange someDateRange{dw::current_date(),
    // dw::current_date()};
};

TEST_F(DISABLED_ExportTasksHandlerFixture, delegates_to_exporter)
{
    // using namespace std::chrono_literals;
    // const std::vector<TaskDTO> tasks{TaskDTO{"123",
    //                                          {"Tag1"},
    //                                          "Some task",
    //                                          5,
    //                                          7,
    //                                          true,
    //                                          current_date_time_local() - 4h},
    //                                  TaskDTO{"345",
    //                                          {"Tag1", "Tag2"},
    //                                          "Another task",
    //                                          2,
    //                                          2,
    //                                          true,
    //                                          current_date_time_local()}};
    // ON_CALL(requestTasksHandlerMock,
    //         handle(api::FinishedTasksQuery{someDateRange}))
    //     .WillByDefault(Return(tasks));
    //
    // EXPECT_CALL(exporterMock,
    //             exportData(::testing::ElementsAreArray(tasks),
    //                        DataFormat::Json,
    //                        SinkType::Network));
    //
    // handler.handle(api::ExportTasksCommand{
    //     someDateRange, DataFormat::Json, SinkType::Network});

    FAIL();
}
