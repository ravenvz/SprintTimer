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
#ifndef SPRINTSTORAGEWRITERMOCK_H_DRJBQM7G
#define SPRINTSTORAGEWRITERMOCK_H_DRJBQM7G

#include <core/ISprintStorageWriter.h>
#include <gmock/gmock.h>

class SprintStorageWriterMock : public sprint_timer::ISprintStorageWriter {
public:
    MOCK_METHOD1(save, void(const sprint_timer::entities::Sprint&));
    MOCK_METHOD1(save,
                 void(const std::vector<sprint_timer::entities::Sprint>&));
    MOCK_METHOD1(remove, void(const sprint_timer::entities::Sprint&));
    MOCK_METHOD1(remove,
                 void(const std::vector<sprint_timer::entities::Sprint>&));
};

#endif /* end of include guard: SPRINTSTORAGEWRITERMOCK_H_DRJBQM7G */
