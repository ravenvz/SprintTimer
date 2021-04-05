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
#ifndef SERIALIZER_H_M8IUPSVG
#define SERIALIZER_H_M8IUPSVG

#include "external_io/SerializationAlgorithm.h"
#include "external_io/SerializationException.h"
#include <core/DataExporter.h>
#include <unordered_map>

namespace sprint_timer::external_io {

template <typename T> class Serializer {
public:
    using AlgoMap =
        std::unordered_map<DataFormat,
                           std::reference_wrapper<SerializationAlgorithm<T>>>;

    explicit Serializer(AlgoMap&& algorithms_)
        : algorithms{std::move(algorithms_)}
    {
    }

    std::string serialize(const T& type, DataFormat format)
    {
        if (auto algorithm = algorithms.find(format);
            algorithm != algorithms.cend()) {
            return algorithm->second.get().serialize(type);
        }
        std::string message{
            "Unable to find serialization algorithm for DataFormat: "};
        message += std::to_string(static_cast<int>(format));
        throw SerializationException{message};
    }

private:
    AlgoMap algorithms;
};

} // namespace sprint_timer::external_io

#endif /* end of include guard: SERIALIZER_H_M8IUPSVG */
