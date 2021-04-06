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
#include <gmock/gmock.h>
#include "qt_gui/presentation/HistoryMediator.h"

namespace mocks {

class HistoryMediatorMock : public sprint_timer::ui::HistoryMediator {
public:
    MOCK_METHOD(void,
                changeDisplayedHistory,
                (sprint_timer::ui::HistoryColleague*,
                 sprint_timer::ui::HistoryMediator::DisplayedHistory),
                (override));

    MOCK_METHOD(void, onRangeChanged, (const dw::DateRange&), (override));

    MOCK_METHOD(std::optional<dw::DateRange>,
                currentDateRange,
                (),
                (const override));

    MOCK_METHOD(sprint_timer::ui::HistoryMediator::DisplayedHistory,
                displayedHistory,
                (),
                (const override));
};

} // namespace mocks
