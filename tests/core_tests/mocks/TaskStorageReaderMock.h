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
#ifndef TASKSTORAGEREADERMOCK_H_P93DAESG
#define TASKSTORAGEREADERMOCK_H_P93DAESG

#include <core/ITaskStorageReader.h>
#include <gmock/gmock.h>

class TaskStorageReaderMock : public sprint_timer::ITaskStorageReader {
public:
    MOCK_METHOD1(requestUnfinishedTasks,
                 void(sprint_timer::ITaskStorageReader::Handler));
    MOCK_METHOD2(requestFinishedTasks,
                 void(const dw::DateRange&,
                      sprint_timer::ITaskStorageReader::Handler));
    MOCK_METHOD2(requestTasks,
                 void(const dw::DateRange&,
                      sprint_timer::ITaskStorageReader::Handler));
    MOCK_METHOD1(requestAllTags,
                 void(sprint_timer::ITaskStorageReader::TagHandler));
};

#endif /* end of include guard: TASKSTORAGEREADERMOCK_H_P93DAESG */
