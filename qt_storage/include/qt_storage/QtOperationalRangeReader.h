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
#ifndef QTOPERATIONALRANGEREADER_H_MNEJCZ5Y
#define QTOPERATIONALRANGEREADER_H_MNEJCZ5Y

#include "core/OperationalRangeReader.h"
#include <QString>

namespace sprint_timer::storage::qt_storage {

class QtOperationalRangeReader : public OperationalRangeReader {
public:
    explicit QtOperationalRangeReader(const QString& connectionName);

    QtOperationalRangeReader(QtOperationalRangeReader&&) = delete;
    QtOperationalRangeReader& operator=(QtOperationalRangeReader&&) = delete;

    QtOperationalRangeReader(const QtOperationalRangeReader&) = delete;
    QtOperationalRangeReader&
    operator=(const QtOperationalRangeReader&) = delete;

    dw::DateRange operationalRange() final;

private:
    const QString& connectionName;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTOPERATIONALRANGEREADER_H_MNEJCZ5Y */

