/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#ifndef TASKVIEWDELEGATE_H_Z8NDXQM6
#define TASKVIEWDELEGATE_H_Z8NDXQM6


#include <QStyledItemDelegate>


class TaskViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TaskViewDelegate(QObject* parent);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    static constexpr int padding {1};
    static constexpr int verticalSpacing {14};

private:

    void paintText(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, const int alpha) const;
    
    QColor tagColor {Qt::blue};
    QColor commonColor {Qt::black};
    QColor overspentColor {Qt::red};
    QColor delimiterColor {Qt::gray};
    const int completedItemAlpha {100};
    const int normalItemAlpha {255};

};

#endif /* end of include guard: TASKVIEWDELEGATE_H_Z8NDXQM6 */
