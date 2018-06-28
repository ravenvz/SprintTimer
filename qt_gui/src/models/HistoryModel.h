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
#ifndef HISTORYMODEL_H_UW24GPQR
#define HISTORYMODEL_H_UW24GPQR

#include <QDate>
#include <QStandardItemModel>
#include <QString>
#include <vector>

#include <iostream>

namespace sprint_timer::ui::qt_gui {

/* Tree model that stores dates and description grouped by date. */
class HistoryModel : public QStandardItemModel {
public:
    using HistoryItem = std::pair<QDate, QString>;
    using HistoryData = std::vector<HistoryItem>;

    HistoryModel(QObject* parent);

    /* Assumes that orderedHistory is sorted by date asc. */
    void fill(const HistoryData& orderedHistory);

    void sortAndfill(HistoryData&& unorderedHistory);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: HISTORYMODEL_H_UW24GPQR */
