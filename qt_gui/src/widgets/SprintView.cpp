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

#include "qt_gui/widgets/SprintView.h"
#include "qt_gui/utils/MouseRightReleaseEater.h"
#include <QMenu>

namespace sprint_timer::ui::qt_gui {

SprintView::SprintView(SprintModel& sprintModel, QWidget* parent)
    : QListView{parent}
    , sprintModel{sprintModel}
{
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setModel(&sprintModel);
    connect(this,
            &QListView::customContextMenuRequested,
            this,
            &SprintView::showContextMenu);
}

void SprintView::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu contextMenu;
    auto mouseRightReleaseEater
        = std::make_unique<MouseRightReleaseEater>(&contextMenu);
    contextMenu.installEventFilter(mouseRightReleaseEater.release());
    const auto deleteEntry = "Delete";
    contextMenu.addAction(deleteEntry);

    QAction* selectedEntry = contextMenu.exec(globalPos);

    if (selectedEntry && selectedEntry->text() == deleteEntry) {
        QModelIndex index = currentIndex();
        sprintModel.remove(index.row());
    }
}

} // namespace sprint_timer::ui::qt_gui
