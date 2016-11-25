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

#ifndef SPRINT_TIMER_CSVWRITER_H
#define SPRINT_TIMER_CSVWRITER_H

#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include "core/entities/Sprint.h"
#include "core/external_io/IDataExporter.h"

namespace ExternalIO {

namespace CSV {

class CSVWriter : public IDataExporter {
public:
    CSVWriter(char delimiter = ',');

    void exportData(ExternalIO::ISink* sink, const std::vector<std::string>& data) override;

private:
    const char delimiter;

    void writeValue(ExternalIO::ISink* sink, const std::string& value);
};

} // namespace CSV

} // namespace ExternalIO

#endif // SPRINT_TIMER_CSVWRITER_H
