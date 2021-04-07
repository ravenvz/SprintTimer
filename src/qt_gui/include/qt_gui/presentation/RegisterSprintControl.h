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
#ifndef REGISTERSPRINTCONTROL_H_1WZJSFJW
#define REGISTERSPRINTCONTROL_H_1WZJSFJW

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include <date_wrapper/date_wrapper.h>
#include <vector>

namespace sprint_timer::ui::contracts::RegisterSprintControl {

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
    virtual void
    registerConsecutiveSprints(const std::string& taskUuid,
                               dw::DateTime firstSprintStart,
                               int32_t numSprints,
                               std::chrono::minutes sprintDuration) = 0;

    virtual void
    registerSprintBulk(const std::string& taskUuid,
                       const std::vector<dw::DateTimeRange>& timeRanges) = 0;
};

class View : public mvp::BaseView<View, Presenter> {
};

} // namespace sprint_timer::ui::contracts::RegisterSprintControl

#endif /* end of include guard: REGISTERSPRINTCONTROL_H_1WZJSFJW */
