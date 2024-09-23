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
#ifndef PLANNERVIEWSPRESENTER_H_AE7IHF5O
#define PLANNERVIEWSPRESENTER_H_AE7IHF5O

#include "qt_gui/presentation/TaskTreeFilter.h"
#include "core/TreeType.h"
#include "qt_gui/presentation/PlannerViewsContract.h"

namespace sprint_timer::ui {

class PlannerViewsPresenter
    : public contracts::PlannerViewsContract::Presenter {
public:
    explicit PlannerViewsPresenter(TaskTreeFilter& taskTreeFilter_);

    auto viewClicked(const std::string& name) -> void override;

private:
    TaskTreeFilter& taskTreeFilter;
    TreeType<ui::contracts::PlannerViewsContract::PlannerViewItem> viewTree;

    auto updateViewImpl() -> void override;

    auto fetchDataImpl() -> void override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: PLANNERVIEWSPRESENTER_H_AE7IHF5O */
