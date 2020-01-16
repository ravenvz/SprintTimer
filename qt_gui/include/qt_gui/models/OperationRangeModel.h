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

#include "qt_gui/DatasyncRelay.h"
#include "qt_gui/models/AsyncListModel.h"
#include <core/QueryHandler.h>
#include <core/use_cases/request_op_range/OperationalRangeQuery.h>

namespace sprint_timer::ui::qt_gui {

class OperationRangeModel : public AsyncListModel {

    Q_OBJECT

public:
    OperationRangeModel(QueryHandler<use_cases::OperationalRangeQuery,
                                     dw::DateRange>& operRangeHandler,
                        DatasyncRelay& datasyncRelay,
                        QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent) const final;

    QVariant data(const QModelIndex& index, int role) const final;

private:
    QueryHandler<use_cases::OperationalRangeQuery, dw::DateRange>&
        operRangeHandler;
    DatasyncRelay& datasyncRelay;
    QStringList storage;

    void requestUpdate() override;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: OPERATIONRANGEMODEL_H_EZACWVF7 */

