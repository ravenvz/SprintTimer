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
#include "qt_gui/models/HistoryModel.h"
#include <memory>

namespace sprint_timer::ui::qt_gui {

HistoryModel::HistoryModel(QObject* parent)
    : QStandardItemModel{parent}
{
}

/* Assumes that orderedHistory is sorted by date asc. */
void HistoryModel::fill(const HistoryData& orderedHistory)
{
    // QStandardItemModel takes ownership of items that are added with
    // appendRow()
    clear();

    if (orderedHistory.empty()) {
        auto item = std::make_unique<QStandardItem>("No data.");
        appendRow(item.release());
        return;
    }

    QStandardItem* parent
        = std::make_unique<QStandardItem>(
              QString("%1 items in total.").arg(orderedHistory.size()))
              .release();
    appendRow(parent);

    for (auto same_date_beg = cbegin(orderedHistory);
         same_date_beg != cend(orderedHistory);) {
        const auto [parent_date, parent_descr] = *same_date_beg;
        auto same_date_end
            = std::find_if(same_date_beg,
                           cend(orderedHistory),
                           [& parent_date = parent_date](const auto& entry) {
                               return entry.first != parent_date;
                           });

        parent = std::make_unique<QStandardItem>(
                     QString("%1 (%2 items)")
                         .arg(parent_date.toString())
                         .arg(std::distance(same_date_beg, same_date_end)))
                     .release();
        appendRow(parent);

        for (int children = 0; same_date_beg != same_date_end;
             ++same_date_beg, ++children) {
            auto item = std::make_unique<QStandardItem>(same_date_beg->second);
            parent->setChild(children, item.release());
        }
    }
}

void HistoryModel::sortAndfill(HistoryData&& unorderedHistory)
{
    auto sortedHistory = std::move(unorderedHistory);
    std::sort(begin(sortedHistory),
              end(sortedHistory),
              [](const auto& lhs, const auto& rhs) {
                  return lhs.second < rhs.second;
              });
    fill(sortedHistory);
}

} // namespace sprint_timer::ui::qt_gui
