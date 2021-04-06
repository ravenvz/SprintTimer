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
#ifndef TODAYPROGRESSCONTRACT_H_PUVI6CYE
#define TODAYPROGRESSCONTRACT_H_PUVI6CYE

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include <string>

namespace sprint_timer::ui::contracts::TodayProgressContract {

class View : public mvp::BaseView<View, mvp::BasePresenter<View>> {
public:
    virtual void displayProgress(const std::string& progressStr,
                                 const std::string& style) = 0;
};

} // namespace sprint_timer::ui::contracts::TodayProgressContract

#endif /* end of include guard: TODAYPROGRESSCONTRACT_H_PUVI6CYE */
