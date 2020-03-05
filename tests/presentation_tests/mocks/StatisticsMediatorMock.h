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
#ifndef STATISTICSMEDIATORMOCK_H_RX2EJZRW
#define STATISTICSMEDIATORMOCK_H_RX2EJZRW

#include "gmock/gmock.h"
#include <qt_gui/presentation/StatisticsMediator.h>

namespace mocks {

class StatisticsMediatorMock : public sprint_timer::ui::StatisticsMediator {
public:
    MOCK_METHOD(void,
                filterByTag,
                (sprint_timer::ui::StatisticsColleague*, std::optional<size_t>),
                (override));

    MOCK_METHOD(void,
                changeRange,
                (sprint_timer::ui::StatisticsColleague*, const dw::DateRange&),
                (override));

    MOCK_METHOD(const std::vector<sprint_timer::entities::Sprint>&,
                sprints,
                (),
                (const override));

    MOCK_METHOD(const std::vector<sprint_timer::TagTop::TagFrequency>&,
                tagFrequencies,
                (),
                (const override));

    MOCK_METHOD(void, changeNumTopTags, (size_t), (override));

    MOCK_METHOD((std::optional<dw::DateRange>), range, (), (const override));

    MOCK_METHOD((std::optional<size_t>),
                selectedTagNumber,
                (),
                (const override));

    MOCK_METHOD(void,
                addColleague,
                (sprint_timer::ui::StatisticsColleague*),
                (override));

    MOCK_METHOD(void,
                removeColleague,
                (sprint_timer::ui::StatisticsColleague*),
                (override));
};

} // namespace mocks

#endif /* end of include guard: STATISTICSMEDIATORMOCK_H_RX2EJZRW */
