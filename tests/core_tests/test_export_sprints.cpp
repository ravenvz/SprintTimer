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
#include "core/use_cases/export_data/ExportSprintsHandler.h"
#include "core/use_cases/request_sprints/RequestSprintsQuery.h"
#include "mocks/DataExporterMock.h"
#include "mocks/QueryHandlerMock.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using namespace sprint_timer;

class ExportSprintsHandlerFixture : public ::testing::Test {
public:
    NiceMock<mocks::QueryHandlerMock<use_cases::RequestSprintsQuery>>
        requestSprintsHandlerMock;
    NiceMock<mocks::DataExporterMock<use_cases::SprintDTO>> exporterMock;
    use_cases::ExportSprintsHandler handler{requestSprintsHandlerMock,
                                            exporterMock};
    const dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
};

TEST_F(ExportSprintsHandlerFixture, delegates_to_exporter)
{
    using use_cases::SprintDTO;
    const dw::DateTimeRange someTimeSpan{dw::current_date_time(),
                                         dw::current_date_time()};
    const std::vector<SprintDTO> sprints{
        SprintDTO{"321", "123", "Some task", {"Tag1", "Tag2"}, someTimeSpan},
        SprintDTO{"654", "456", "Other task", {"Tag7"}, someTimeSpan}};
    ON_CALL(requestSprintsHandlerMock,
            handle(sprint_timer::use_cases::RequestSprintsQuery{someDateRange}))
        .WillByDefault(Return(sprints));

    EXPECT_CALL(exporterMock,
                exportData(sprints, DataFormat::Json, SinkType::Network));

    handler.handle(use_cases::ExportSprintsCommand{
        someDateRange, DataFormat::Json, SinkType::Network});
}
