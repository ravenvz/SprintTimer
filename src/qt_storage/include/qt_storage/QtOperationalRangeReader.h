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
#ifndef QTOPERATIONALRANGEREADER_H_MNEJCZ5Y
#define QTOPERATIONALRANGEREADER_H_MNEJCZ5Y

#include "api/OperationalRangeReader.h"
#include "qt_storage/utils/DateTimeConverter.h"

#include <QString>

namespace sprint_timer::storage::qt_storage {

class QtOperationalRangeReader : public OperationalRangeReader {
public:
    explicit QtOperationalRangeReader(QString connectionName);

    QtOperationalRangeReader(QtOperationalRangeReader&&) = delete;

    QtOperationalRangeReader(const QtOperationalRangeReader&) = delete;

    auto operator=(const QtOperationalRangeReader&)
        -> QtOperationalRangeReader& = delete;

    auto operator=(QtOperationalRangeReader&&)
        -> QtOperationalRangeReader& = delete;

    auto operationalRange() -> dw::DateRange final;

private:
    QString connectionName;
    sprint_timer::storage::utils::DateConverter dateConverter;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTOPERATIONALRANGEREADER_H_MNEJCZ5Y */

