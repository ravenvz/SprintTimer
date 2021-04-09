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
#ifndef TASKSPRINTSCONTRACT_H_FEDQDWT1
#define TASKSPRINTSCONTRACT_H_FEDQDWT1

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include "qt_gui/presentation/SprintDTO.h"
#include <vector>

namespace sprint_timer::ui::contracts::TaskSprintsContract {

class View : public mvp::BaseView<View, mvp::BasePresenter<View>> {
public:
    virtual void displaySprints(const std::vector<SprintDTO>& sprints) = 0;
};

} // namespace sprint_timer::ui::contracts::TaskSprintsContract

#endif /* end of include guard: TASKSPRINTSCONTRACT_H_FEDQDWT1 */
