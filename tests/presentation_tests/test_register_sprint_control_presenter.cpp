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
#include "core/use_cases/register_sprint/RegisterSprintBulkCommand.h"
#include "mocks/CommandHandlerMock.h"
#include "qt_gui/presentation/RegisterSprintControlPresenter.h"

using sprint_timer::use_cases::RegisterSprintBulkCommand;
using ::testing::NiceMock;

namespace {

} // namespace

class RegisterSprintControlPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::CommandHandlerMock<RegisterSprintBulkCommand>>
        registerSprintBulkHandler;
    sprint_timer::ui::RegisterSprintControlPresenter sut{
        registerSprintBulkHandler};
};

TEST_F(RegisterSprintControlPresenterFixture,
       invokes_register_sprint_bulk_handler_for_consecutive_sprints)
{
    using namespace std::chrono_literals;
    using namespace dw;
    const std::string taskUuid{"123"};
    const dw::DateTime firstSprintStartTime{
        DateTime{Date{Year{2021}, Month{3}, Day{3}}} + 3h};
    const dw::DateTimeRange firstSprintRange{firstSprintStartTime,
                                             firstSprintStartTime + 25min};

    const std::vector<DateTimeRange> expectedRanges{
        firstSprintRange,
        add_offset(firstSprintRange, 25min),
        add_offset(firstSprintRange, 50min),
        add_offset(firstSprintRange, 75min)};

    EXPECT_CALL(registerSprintBulkHandler,
                handle(RegisterSprintBulkCommand{taskUuid, expectedRanges}));

    sut.registerConsecutiveSprints(taskUuid, firstSprintStartTime, 4, 25min);
}

TEST_F(RegisterSprintControlPresenterFixture,
       invokes_register_sprint_bulk_handler_for_batched_ranges)
{
    using namespace std::chrono_literals;
    using namespace dw;
    const std::string taskUuid{"123"};
    const dw::DateTime firstSprintStartTime{
        DateTime{Date{Year{2021}, Month{3}, Day{3}}} + 3h};
    const dw::DateTimeRange firstSprintRange{firstSprintStartTime,
                                             firstSprintStartTime + 25min};
    const std::vector<DateTimeRange> expectedRanges{
        firstSprintRange,
        add_offset(firstSprintRange, 25min),
        add_offset(firstSprintRange, 50min),
        add_offset(firstSprintRange, 75min)};

    EXPECT_CALL(registerSprintBulkHandler,
                handle(RegisterSprintBulkCommand{taskUuid, expectedRanges}));

    sut.registerSprintBulk(taskUuid, expectedRanges);
}
