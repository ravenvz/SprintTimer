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
    MOCK_METHOD1(save, void(const sprint_timer::entities::Sprint&));
    MOCK_METHOD1(save,
                 void(const std::vector<sprint_timer::entities::Sprint>&));
    MOCK_METHOD1(remove, void(const sprint_timer::entities::Sprint&));
    MOCK_METHOD1(remove,
                 void(const std::vector<sprint_timer::entities::Sprint>&));
    MOCK_METHOD1(
        findByDateRange,
        std::vector<sprint_timer::entities::Sprint>(const dw::DateRange&));
    MOCK_METHOD1(
        findByTaskUuid,
        std::vector<sprint_timer::entities::Sprint>(const std::string&));
};

} // namespace mocks

#endif /* end of include guard: SPRINTSTORAGEMOCK_H_FQK62RLC */
