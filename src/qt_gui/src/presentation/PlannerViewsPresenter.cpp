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
#include "cpp_utils/algorithms/optional_ext.h"

namespace sprint_timer::ui {

PlannerViewsPresenter::PlannerViewsPresenter(TaskTreeFilter& taskTreeFilter_)
    : taskTreeFilter{taskTreeFilter_}
{
}

auto PlannerViewsPresenter::viewClicked(const std::string& name) -> void
{
    if (name == "All Tasks") {
        taskTreeFilter.clear();
    }
    else {
        taskTreeFilter.select(name);
    }
}

auto PlannerViewsPresenter::updateViewImpl() -> void
{
    alg::inspect(view(), [this](auto* v) { v->displayViews(viewTree); });
}

auto PlannerViewsPresenter::fetchDataImpl() -> void
{
    TreeType<ui::contracts::PlannerViewsContract::PlannerViewItem> treeStub;
    treeStub.insert(treeStub.end(), {"Inbox", ""});
    treeStub.insert(treeStub.end(), {"Starred", ""});
    auto outline = treeStub.insert(treeStub.end(), {"Outline", ""});
    treeStub.insert(outline, {"All Tasks", ""});
    treeStub.insert(outline, {"Projects", ""});
    treeStub.insert(outline, {"Goals", ""});
    treeStub.insert(outline, {"Review", ""});
    auto todo = treeStub.insert(treeStub.end(), {"TODO", ""});
    treeStub.insert(todo, {"Active Actions", ""});
    treeStub.insert(todo, {"Active By Tag", ""});
    treeStub.insert(todo, {"Active By Project", ""});
    treeStub.insert(todo, {"Due next 7 days", ""});
    treeStub.insert(todo, {"Missed reminders", ""});
    auto recent = treeStub.insert(treeStub.end(), {"Recent", ""});
    treeStub.insert(recent, {"Completed recently", ""});
    treeStub.insert(recent, {"Modified recently", ""});

    viewTree = treeStub;
}

} // namespace sprint_timer::ui
