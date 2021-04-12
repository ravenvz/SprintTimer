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
#ifndef TASKSTORAGEMOCK_H_EQEDPXT6
#define TASKSTORAGEMOCK_H_EQEDPXT6

#include "core/TaskStorage.h"
#include "gmock/gmock.h"

namespace mocks {

class TaskStorageMock : public sprint_timer::TaskStorage {
public:
    MOCK_METHOD1(save, void(const sprint_timer::entities::Task&));
    MOCK_METHOD1(remove, void(const std::string&));
    MOCK_METHOD2(edit,
                 void(const sprint_timer::entities::Task&,
                      const sprint_timer::entities::Task&));
    MOCK_METHOD2(toggleCompleted,
                 void(const std::string&, const dw::DateTime&));
    MOCK_METHOD1(updatePriorities, void(const std::vector<std::string>&));
    MOCK_METHOD2(editTag, void(const std::string&, const std::string&));
    MOCK_METHOD0(unfinishedTasks, std::vector<sprint_timer::entities::Task>());
    MOCK_METHOD1(
        finishedTasks,
        std::vector<sprint_timer::entities::Task>(const dw::DateRange&));
    MOCK_METHOD1(
        allTasks,
        std::vector<sprint_timer::entities::Task>(const dw::DateRange&));
    MOCK_METHOD0(allTags, std::vector<std::string>());
};

} // namespace mocks

#endif /* end of include guard: TASKSTORAGEMOCK_H_EQEDPXT6 */
