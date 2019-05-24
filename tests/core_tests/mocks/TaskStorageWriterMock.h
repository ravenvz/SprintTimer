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
#ifndef TASKSTORAGEWRITERMOCK_H_K4RA0LHC
#define TASKSTORAGEWRITERMOCK_H_K4RA0LHC

#include <core/ITaskStorageWriter.h>
#include <gmock/gmock.h>

class TaskStorageWriterMock : public sprint_timer::ITaskStorageWriter {
public:
    MOCK_METHOD1(save, void(const sprint_timer::entities::Task&));
    MOCK_METHOD1(remove, void(const std::string&));
    MOCK_METHOD2(edit,
                 void(const sprint_timer::entities::Task&,
                      const sprint_timer::entities::Task&));
    MOCK_METHOD1(incrementSprints, void(const std::string&));
    MOCK_METHOD1(decrementSprints, void(const std::string&));
    MOCK_METHOD2(toggleTaskCompletionStatus,
                 void(const std::string&, const dw::DateTime&));
    MOCK_METHOD1(updatePriorities, void(const std::vector<std::string>&));
    MOCK_METHOD2(editTag, void(const std::string&, const std::string&));
};

#endif /* end of include guard: TASKSTORAGEWRITERMOCK_H_K4RA0LHC */
