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
#ifndef PLANNERCONTRACT_H_FYB5GGKL
#define PLANNERCONTRACT_H_FYB5GGKL

#include "api/dtos/TaskTreeDTO.h"
#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"

namespace sprint_timer::ui::contracts::PlannerContract {

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual void displayPlanner(const api::TaskTreeDTO& taskTree) = 0;
};

} // namespace sprint_timer::ui::contracts::PlannerContract

#endif /* end of include guard: PLANNERCONTRACT_H_FYB5GGKL */

