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
#ifndef TASKITEMDELEGATE_H_Z8NDXQM6
#define TASKITEMDELEGATE_H_Z8NDXQM6

#include <QStyledItemDelegate>

namespace sprint_timer::ui::qt_gui {


class TaskItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit TaskItemDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;

private:
    void paintItem(QPainter* painter,
                   const QStyleOptionViewItem& option,
                   const QModelIndex& index,
                   int alpha) const;

    QColor tagColor{Qt::blue};
    QColor commonColor{Qt::black};
    QColor overspentColor{Qt::red};
    QColor delimiterColor{Qt::gray};
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: TASKITEMDELEGATE_H_Z8NDXQM6 */
