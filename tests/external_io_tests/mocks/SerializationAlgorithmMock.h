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
#ifndef SERIALIZATIONALGORITHMMOCK_H_BJ9DZY5T
#define SERIALIZATIONALGORITHMMOCK_H_BJ9DZY5T

#include <external_io/SerializationAlgorithm.h>
#include <gmock/gmock.h>

namespace mocks {

template <typename T>
class SerializationAlgorithmMock
    : public sprint_timer::external_io::SerializationAlgorithm<T> {
public:
    MOCK_METHOD(std::string, serialize, (const T&), (override));

    MOCK_METHOD(std::vector<std::string>, serializeBatch, (const std::vector<T>&), (override));
};

} // namespace mocks

#endif /* end of include guard: SERIALIZATIONALGORITHMMOCK_H_BJ9DZY5T */
