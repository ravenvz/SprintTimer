/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#ifndef UNFINISHEDTASKSCONTRACT_H_3TEOUIV4
#define UNFINISHEDTASKSCONTRACT_H_3TEOUIV4

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"

namespace sprint_timer::ui::contracts::TaskViewContract {

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
    virtual void changeTaskSelection(size_t index, std::string&& uuid) = 0;
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual void selectTask(std::optional<size_t> taskIndex) = 0;
};

} // namespace sprint_timer::ui::contracts::TaskViewContract

#endif /* end of include guard: UNFINISHEDTASKSCONTRACT_H_3TEOUIV4 */
