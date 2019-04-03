/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#ifndef EXTRADAYMODEL_H_CC87JAHL
#define EXTRADAYMODEL_H_CC87JAHL

#include "qt_gui/utils/DateTimeConverter.h"
#include <QAbstractListModel>
#include <QDate>
#include <algorithm>
#include <core/CommandInvoker.h>
#include <core/IWorkingDaysReader.h>
#include <core/IWorkingDaysWriter.h>
#include <core/QueryInvoker.h>
#include <core/WorkdayTracker.h>

namespace sprint_timer::ui::qt_gui {

class ExtraDayModel : public QAbstractListModel {
public:
    explicit ExtraDayModel(QObject* parent = nullptr);

    void setExtraDayData(const std::vector<dw::Date>& days);

    int rowCount(const QModelIndex& parent = QModelIndex()) const final;

    bool removeRows(int row,
                    int count,
                    const QModelIndex& index = QModelIndex()) final;

    QVariant data(const QModelIndex& index, int role) const final;

    void insert(const dw::Date& date);

    dw::Date dateAt(int row);

protected:
    std::vector<dw::Date> data_;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: EXTRADAYMODEL_H_CC87JAHL */
