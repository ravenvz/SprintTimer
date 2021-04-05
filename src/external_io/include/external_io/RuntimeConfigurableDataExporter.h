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
#ifndef SPRINTDATAEXPORTER_H_ZDYS5FI6
#define SPRINTDATAEXPORTER_H_ZDYS5FI6

#include "external_io/RuntimeSinkRouter.h"
#include "external_io/Serializer.h"
#include <core/DataExporter.h>
#include <core/entities/Sprint.h>

namespace sprint_timer::external_io {

template <typename T>
class RuntimeConfigurableDataExporter : public DataExporter<T> {
public:
    RuntimeConfigurableDataExporter(Serializer<T>& serializer,
                                    RuntimeSinkRouter& router)
        : serializer{serializer}
        , router{router}
    {
    }

    void exportData(const std::vector<T>& data,
                    DataFormat format,
                    SinkType sinkType) override
    {
        std::vector<std::string> serializedItems;
        serializedItems.reserve(data.size());
        for (const auto& item : data) {
            serializedItems.push_back(serializer.serialize(item, format));
        }
        router.route(std::move(serializedItems), sinkType);
    }

private:
    Serializer<T>& serializer;
    RuntimeSinkRouter& router;
};

} // namespace sprint_timer::external_io

#endif /* end of include guard: SPRINTDATAEXPORTER_H_ZDYS5FI6 */
