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

#include "mocks/SprintStorageMock.h"
#include "gtest/gtest.h"
#include <core/ObservableActionInvoker.h>
#include <core/actions/DeleteSprint.h>

using sprint_timer::actions::DeleteSprint;
using sprint_timer::entities::Sprint;
using sprint_timer::entities::Tag;
using namespace dw;

using ::testing::_;

class DeleteSprintFixture : public ::testing::Test {
public:
    sprint_timer::ObservableActionInvoker actionInvoker;
    mocks::SprintStorageMock sprint_storage_mock;

    const DateTimeRange someTimeSpan{add_offset(
        DateTimeRange{current_date_time(), current_date_time()}, Days{-1})};
    const Sprint someSprint{"Task name",
                            someTimeSpan,
                            {Tag{"Tag1"}, Tag{"Tag2"}},
                            "550e8400-e29b-41d4-a716-446655440000",
                            "722e8400"};
};

TEST_F(DeleteSprintFixture, execute_and_undo)
{
    EXPECT_CALL(sprint_storage_mock, remove(someSprint)).Times(1);

    actionInvoker.execute(
        std::make_unique<DeleteSprint>(sprint_storage_mock, someSprint));

    EXPECT_CALL(sprint_storage_mock, save(someSprint)).Times(1);

    actionInvoker.undo();
}
