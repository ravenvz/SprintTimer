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
#ifndef TASKSELECTIONCOLLEAGUEMOCK_H_USODC6KX
#define TASKSELECTIONCOLLEAGUEMOCK_H_USODC6KX

#include "gmock/gmock.h"
#include "qt_gui/presentation/TaskSelectionMediator.h"

namespace mocks {

class TaskSelectionColleagueMock
    : public sprint_timer::ui::TaskSelectionColleague {
public:
    MOCK_METHOD(void, onTaskSelectionChanged, (), (override));
};

} // namespace mocks

#endif /* end of include guard: TASKSELECTIONCOLLEAGUEMOCK_H_USODC6KX */
