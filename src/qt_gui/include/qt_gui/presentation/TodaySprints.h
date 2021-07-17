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
#ifndef TODAYSPRINTS_H_V0K2AYQR
#define TODAYSPRINTS_H_V0K2AYQR

#include "core/use_cases/SprintDTO.h"
#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"

namespace sprint_timer::ui::contracts::TodaySprints {

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
    virtual void onSprintDelete(const std::string& uuid) = 0;
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual void displaySprints(const std::vector<use_cases::SprintDTO>&) = 0;
};

} // namespace sprint_timer::ui::contracts::TodaySprints

#endif /* end of include guard: TODAYSPRINTS_H_V0K2AYQR */
