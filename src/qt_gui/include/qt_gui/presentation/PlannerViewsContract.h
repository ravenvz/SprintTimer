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
#include "core/TreeType.h"
#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"

#ifndef PLANNERVIEWSCONTRACT_H_QUSKLZBW
#define PLANNERVIEWSCONTRACT_H_QUSKLZBW

namespace sprint_timer::ui::contracts::PlannerViewsContract {

struct PlannerViewItem {
    std::string name;
    std::string iconPath;
};

using ViewsTree = TreeType<PlannerViewItem>;

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
    virtual auto viewClicked(const std::string& viewName) -> void = 0;
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual auto displayViews(const ViewsTree& viewsTree) -> void = 0;
};

} // namespace sprint_timer::ui::contracts::PlannerViewsContract

#endif /* end of include guard: PLANNERVIEWSCONTRACT_H_QUSKLZBW */
