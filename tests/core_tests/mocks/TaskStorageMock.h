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
    MOCK_METHOD(void, save, (const sprint_timer::entities::Task&), (override));
    MOCK_METHOD(void, remove, (const std::string&), (override));
    MOCK_METHOD(void,
                edit,
                (const sprint_timer::entities::Task&,
                 const sprint_timer::entities::Task&),
                (override));
    MOCK_METHOD(void,
                toggleCompleted,
                (const std::string&, const dw::DateTime&),
                (override));
    MOCK_METHOD(void,
                updatePriorities,
                (const std::vector<std::string>&),
                (override));
    MOCK_METHOD(void,
                editTag,
                (const std::string&, const std::string&),
                (override));
    MOCK_METHOD((std::vector<sprint_timer::entities::Task>),
                unfinishedTasks,
                (),
                (override));
    MOCK_METHOD((std::vector<sprint_timer::entities::Task>),
                finishedTasks,
                (const dw::DateRange&),
                (override));
    MOCK_METHOD((std::vector<sprint_timer::entities::Task>),
                allTasks,
                (const dw::DateRange&),
                (override));
    MOCK_METHOD((std::vector<std::string>), allTags, (), (override));
    MOCK_METHOD((std::vector<sprint_timer::entities::Task>),
                findByUuid,
                (const std::string&),
                (override));
};

} // namespace mocks

#endif /* end of include guard: TASKSTORAGEMOCK_H_EQEDPXT6 */
