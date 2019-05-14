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
#include "qt_gui/models/OperationRangeModel.h"
#include <core/use_cases/RequestOperationalRange.h>

namespace sprint_timer::ui::qt_gui {

OperationRangeModel::OperationRangeModel(IOperationalRangeReader& reader_,
                                         QueryInvoker& queryInvoker_)
    : reader{reader_}
    , queryInvoker{queryInvoker_}
{
    requestDataUpdate();
}

dw::DateRange OperationRangeModel::operationRange() const
{
    return min_max_date;
}

void OperationRangeModel::requestDataUpdate()
{
    using use_cases::RequestOperationalRange;
    queryInvoker.execute(std::make_unique<RequestOperationalRange>(
        reader, [this](const auto& updatedRange) {
            min_max_date = updatedRange;
            emit operationRangeUpdated(min_max_date);
        }));
}

} // namespace sprint_timer::ui::qt_gui
