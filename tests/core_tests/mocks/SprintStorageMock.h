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
#ifndef SPRINTSTORAGEMOCK_H_FQK62RLC
#define SPRINTSTORAGEMOCK_H_FQK62RLC

#include "core/SprintStorage.h"
#include "gmock/gmock.h"

namespace mocks {

class SprintStorageMock : public sprint_timer::SprintStorage {
public:
    MOCK_METHOD(void,
                save,
                (const sprint_timer::entities::Sprint&),
                (override));
    MOCK_METHOD(void,
                save,
                (const std::vector<sprint_timer::entities::Sprint>&),
                (override));
    MOCK_METHOD(void,
                remove,
                (const sprint_timer::entities::Sprint&),
                (override));
    MOCK_METHOD(void,
                remove,
                (const std::vector<sprint_timer::entities::Sprint>&),
                (override));
    MOCK_METHOD(std::vector<sprint_timer::entities::Sprint>,
                findByDateRange,
                (const dw::DateRange&),
                (override));
    MOCK_METHOD(std::vector<sprint_timer::entities::Sprint>,
                findByTaskUuid,
                (const std::string&),
                (override));
    MOCK_METHOD(std::vector<sprint_timer::entities::Sprint>,
                findByUuid,
                (const std::string&),
                (override));
};

} // namespace mocks

#endif /* end of include guard: SPRINTSTORAGEMOCK_H_FQK62RLC */
