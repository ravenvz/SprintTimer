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
#include "qt_gui/presentation/PlannerViewsPresenter.h"
#include "gmock/gmock.h"

using sprint_timer::api::TaskTreeDTO;
using sprint_timer::ui::PlannerViewsPresenter;
using sprint_timer::ui::TaskTreeFilter;
using sprint_timer::ui::contracts::PlannerViewsContract::ViewsTree;

class PlannerViewMock
    : public sprint_timer::ui::contracts::PlannerViewsContract::View {
public:
    MOCK_METHOD(void, displayViews, ((const ViewsTree&)), (override));
};

class SomeFixture : public ::testing::Test {
public:
    TaskTreeFilter taskTreeFilter{TaskTreeFilter::FilterMap{
        {"filter1", [](const TaskTreeDTO& tree) { return tree; }},
        {"filter1", [](const TaskTreeDTO& tree) { return tree; }}}};
    PlannerViewsPresenter sut{taskTreeFilter};
};

TEST_F(SomeFixture, selects_filter)
{
    sut.viewClicked("filter1");

    const auto filter = taskTreeFilter.currentFilter();

    ASSERT_TRUE(filter.has_value());
    ASSERT_EQ("filter1", taskTreeFilter.currentFilter().value());
}

TEST_F(SomeFixture, clears_filter_when_all_tasks_are_selected)
{
    sut.viewClicked("filter1");
    sut.viewClicked("All Tasks");

    const auto filter = taskTreeFilter.currentFilter();

    ASSERT_FALSE(filter.has_value());
}
