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
#ifndef YEARRANGEREADERMOCK_H_RFGJ7KUC
#define YEARRANGEREADERMOCK_H_RFGJ7KUC

#include <core/IYearRangeReader.h>
#include <gmock/gmock.h>

class YearRangeReaderMock : public sprint_timer::IYearRangeReader {
public:
    MOCK_METHOD1(requestYearRange,
                 void(sprint_timer::IYearRangeReader::Handler));
};

#endif /* end of include guard: YEARRANGEREADERMOCK_H_RFGJ7KUC */
