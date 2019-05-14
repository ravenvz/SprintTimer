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
#ifndef OPERATIONRANGEMODEL_H_EZACWVF7
#define OPERATIONRANGEMODEL_H_EZACWVF7

#include <QObject>
#include <core/IOperationalRangeReader.h>
#include <core/QueryInvoker.h>

namespace sprint_timer::ui::qt_gui {

class OperationRangeModel : public QObject {

    Q_OBJECT

public:
    OperationRangeModel(IOperationalRangeReader& yearRangeReader,
                        QueryInvoker& queryInvoker);

    dw::DateRange operationRange() const;

    void requestDataUpdate();

signals:
    void operationRangeUpdated(const dw::DateRange&);

private:
    IOperationalRangeReader& reader;
    QueryInvoker& queryInvoker;
    dw::DateRange min_max_date{dw::current_date(), dw::current_date()};
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: OPERATIONRANGEMODEL_H_EZACWVF7 */

