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
#ifndef CSVENCODER_H_GKMATYII
#define CSVENCODER_H_GKMATYII

#include "core/entities/Sprint.h"
#include "external_io/SerializationAlgorithm.h"
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace sprint_timer::external_io {

std::string encode(const std::vector<std::string>& records, char delimiter);

template <typename T>
class CsvSerializationAlgorithm : public SerializationAlgorithm<T> {
public:
    std::string serialize(const T& type) override
    {
        return encode(toRecords(type), delimiter);
    }

    std::vector<std::string>
    serializeBatch(const std::vector<T>& batch) override
    {
        std::vector<std::string> encoded;
        encoded.reserve(batch.size());
        std::transform(cbegin(batch),
                       cend(batch),
                       std::back_inserter(encoded),
                       [this](const auto& type) { return serialize(type); });
        return encoded;
    }

private:
    char delimiter{';'};

    virtual std::vector<std::string> toRecords(const T& type) const = 0;
};

} // namespace sprint_timer::external_io

#endif /* end of include guard: CSVENCODER_H_GKMATYII */

