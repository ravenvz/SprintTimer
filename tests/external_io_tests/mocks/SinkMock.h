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
#ifndef SINKMOCK_H_XWWR7ZEO
#define SINKMOCK_H_XWWR7ZEO

#include "external_io/Sink.h"
#include "gmock/gmock.h"

namespace mocks {

class SinkMock : public sprint_timer::external_io::Sink {
public:
    MOCK_METHOD(void, send, (std::vector<std::string> &&), (override));
};

} // namespace mocks

#endif /* end of include guard: SINKMOCK_H_XWWR7ZEO */
