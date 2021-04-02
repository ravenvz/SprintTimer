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
#ifndef SERIALIZATIONALGORITHM_H_8QLOKX1N
#define SERIALIZATIONALGORITHM_H_8QLOKX1N

#include <string>
#include <vector>

namespace sprint_timer::external_io {

template <typename T> class SerializationAlgorithm {
public:
    virtual std::string serialize(const T& type) = 0;

    virtual std::vector<std::string>
    serializeBatch(const std::vector<T>& batch) = 0;

    SerializationAlgorithm() = default;
    virtual ~SerializationAlgorithm() = default;
    SerializationAlgorithm(const SerializationAlgorithm& other) = delete;
    SerializationAlgorithm&
    operator=(const SerializationAlgorithm& other) = delete;
    SerializationAlgorithm(SerializationAlgorithm&& other) noexcept = delete;
    SerializationAlgorithm&
    operator=(SerializationAlgorithm&& other) noexcept = delete;
};

} // namespace sprint_timer::external_io

#endif /* end of include guard: SERIALIZATIONALGORITHM_H_8QLOKX1N */
