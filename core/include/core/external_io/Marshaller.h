/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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

#ifndef SPRINT_TIMER_EXTERNALIO_H
#define SPRINT_TIMER_EXTERNALIO_H

#include "core/entities/Sprint.h"
#include "core/external_io/IDataExporter.h"
#include "core/external_io/ISink.h"
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>

namespace ExternalIO {

class Marshaller {
public:
    Marshaller(std::shared_ptr<IDataExporter> exporter,
               std::shared_ptr<ISink> sink)
        : exporter{exporter}
        , sink{sink}
    {
    }

    template <typename Entity, typename Func>
    void marshall(const std::vector<Entity>& data, Func func)
    {
        for (const auto& el : data) {
            exporter->exportData(sink.get(), func(el));
        }
    }

private:
    std::shared_ptr<IDataExporter> exporter;
    std::shared_ptr<ISink> sink;
};

} // namespace ExternalIO

#endif // SPRINT_TIMER_EXTERNALIO_H
