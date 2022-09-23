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
#ifndef EDITTASKDIALOGCONTRACT_H_NCV0N5CX
#define EDITTASKDIALOGCONTRACT_H_NCV0N5CX

#include "api/dtos/TaskDTO.h"
#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"

namespace sprint_timer::ui::contracts::EditTaskDialogContract {

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
    virtual auto onEditTaskAccepted(api::TaskDTO&& editedTask) -> void = 0;
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual auto fillTaskDetails(const api::TaskDTO& original) -> void = 0;

    virtual auto fillTags(std::span<const std::string> tags) -> void = 0;
};

} // namespace sprint_timer::ui::contracts::EditTaskDialogContract

#endif /* end of include guard: EDITTASKDIALOGCONTRACT_H_NCV0N5CX */
