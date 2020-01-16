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
#ifndef OPERATIONALRANGEREADERMOCK_H_APJ2MZW1
#define OPERATIONALRANGEREADERMOCK_H_APJ2MZW1

#include "gmock/gmock.h"
#include <core/OperationalRangeReader.h>

namespace mocks {

class OperationalRangeReaderMock : public sprint_timer::OperationalRangeReader {
public:
    MOCK_METHOD0(operationalRange, dw::DateRange());
};

} // namespace mocks

#endif /* end of include guard: OPERATIONALRANGEREADERMOCK_H_APJ2MZW1 */
