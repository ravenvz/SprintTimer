/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "qt_gui/widgets/ReordableTreeView.h"
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QEvent>
#include <QTreeView>

namespace sprint_timer::ui::qt_gui {

ReordableTreeView::ReordableTreeView(QWidget* parent)
    : QTreeView{parent}
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

auto ReordableTreeView::dragMoveEvent(QDragMoveEvent* event) -> void
{
    setDropIndicatorShown(true);
    QTreeView::dragMoveEvent(event);
}

auto ReordableTreeView::dropEvent(QDropEvent* event) -> void
{
    QTreeView::dropEvent(event);

    auto action = event->dropAction();

    if (action == Qt::IgnoreAction) {
        return;
    }
    if (action != Qt::MoveAction) {
        return;
    }

    const auto dropIndicator = dropIndicatorPosition();

    const auto destinationIndex = indexAt(event->position().toPoint());
    int destinationRow{destinationIndex.row()};

    const auto from = currentIndex();
    const auto fromParent = model()->parent(from);

    selectionModel()->clear();

    switch (dropIndicator) {
    case QAbstractItemView::AboveItem:
        model()->moveRows(fromParent,
                          from.row(),
                          1,
                          model()->parent(destinationIndex),
                          destinationRow);
        break;
    case QAbstractItemView::BelowItem:
        model()->moveRows(fromParent,
                          from.row(),
                          1,
                          model()->parent(destinationIndex),
                          destinationRow + 1);
        break;
    case QAbstractItemView::OnItem:
        model()->moveRows(fromParent,
                          from.row(),
                          1,
                          destinationIndex,
                          destinationIndex.model()->rowCount(destinationIndex));
        break;
    case QAbstractItemView::OnViewport:
        model()->moveRows(fromParent,
                          from.row(),
                          1,
                          QModelIndex(),
                          model()->rowCount(QModelIndex()));
        break;
    }

    setDropIndicatorShown(false);
}

} // namespace sprint_timer::ui::qt_gui
