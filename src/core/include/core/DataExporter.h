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
#ifndef DATAEXPORTER_H_PRTTGREQ
#define DATAEXPORTER_H_PRTTGREQ

#include <vector>

namespace sprint_timer {

enum class DataFormat { Csv, Json };

enum class SinkType { Stdout, File, Network };

template <typename T> class DataExporter {
public:
    virtual void exportData(const std::vector<T>& data,
                            DataFormat format,
                            SinkType sink) = 0;

    DataExporter() = default;
    virtual ~DataExporter() = default;
    DataExporter(const DataExporter& other) = delete;
    DataExporter& operator=(const DataExporter& other) = delete;
    DataExporter(DataExporter&& other) noexcept = delete;
    DataExporter& operator=(DataExporter&& other) noexcept = delete;
};

} // namespace sprint_timer

#endif /* end of include guard: DATAEXPORTER_H_PRTTGREQ */
