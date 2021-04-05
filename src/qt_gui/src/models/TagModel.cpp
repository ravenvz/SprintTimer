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
#include "qt_gui/models/TagModel.h"

namespace sprint_timer::ui::qt_gui {

TagModel::TagModel(QObject* parent_)
    : QStringListModel{parent_}
{
}

inline void TagModel::displayTags(const std::vector<std::string>& tags)
{
    QStringList data;
    std::transform(
        cbegin(tags),
        cend(tags),
        std::back_inserter(data),
        [](const auto& elem) { return QString::fromStdString(elem); });
    setStringList(data);
}

bool TagModel::setData(const QModelIndex& index,
                       const QVariant& value,
                       int role)
{
    if (!index.isValid())
        return false;
    if (role == Qt::EditRole) {
        buffer.push_back({data(index, role).toString(), value.toString()});
        return true;
    }
    return false;
}

bool TagModel::submit()
{
    if (auto p = presenter(); p) {
        while (!buffer.empty()) {
            p.value()->renameTag(buffer.back().first.toStdString(),
                                 buffer.back().second.toStdString());
            buffer.pop_back();
        }
        return true;
    }
    return false;
}

void TagModel::revert() { buffer.clear(); }

} // namespace sprint_timer::ui::qt_gui
