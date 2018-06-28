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

#include "SubmissionItemDelegate.h"
#include <QPainter>

namespace sprint_timer::ui::qt_gui {

SubmissionItemDelegate::SubmissionItemDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
}

void SubmissionItemDelegate::paint(QPainter* painter,
                                   const QStyleOptionViewItem& option,
                                   const QModelIndex& index) const
{
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
    painter->save();
    if (index.data(Qt::CheckStateRole).toBool()) {
        QFont completedTaskFont{option.font};
        completedTaskFont.setStrikeOut(true);
        painter->setFont(completedTaskFont);
    }
    painter->drawText(option.rect, Qt::AlignLeft, index.data(Qt::DisplayRole).toString());
    painter->restore();
}

} // namespace sprint_timer::ui::qt_gui

