/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "widgets/SprintListView.h"

SprintListView::SprintListView(QWidget* parent)
    : QListView{parent}
{
    connect(this,
            &QListView::customContextMenuRequested,
            this,
            &SprintListView::showContextMenu);
}

QSize SprintListView::sizeHint() const { return QSize(300, 200); }

void SprintListView::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu contextMenu;
    contextMenu.addAction("Delete");

    QAction* selectedItem = contextMenu.exec(globalPos);

    if (selectedItem && selectedItem->text() == "Delete")
        removeItem();
}

void SprintListView::removeItem()
{
    QModelIndex index = currentIndex();
    ConfirmationDialog dialog;
    QString description{"This will remove sprint permanently"};
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        dynamic_cast<SprintModel*>(model())->remove(index.row());
    }
}