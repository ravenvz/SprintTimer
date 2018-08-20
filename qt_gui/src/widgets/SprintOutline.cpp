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
#include "qt_gui/widgets/SprintOutline.h"
#include "qt_gui/dialogs/AddSprintDialog.h"
#include "qt_gui/dialogs/UndoDialog.h"
#include "qt_gui/models/SprintModel.h"
#include "qt_gui/utils/MouseRightReleaseEater.h"
#include <QMenu>
#include <QVBoxLayout>

namespace sprint_timer::ui::qt_gui {

SprintOutline::SprintOutline(SprintModel& sprintModel,
                             AddSprintDialog& addSprintDialog,
                             UndoDialog& undoDialog,
                             std::unique_ptr<QPushButton> undoButton,
                             std::unique_ptr<QPushButton> addNewSprintButton,
                             QWidget* parent)
    : QWidget{parent}
    , sprintModel{sprintModel}
{
    QVBoxLayout* layout = new QVBoxLayout{this};

    connect(undoButton.get(), &QPushButton::clicked, [&undoDialog]() {
        undoDialog.exec();
    });
    layout->addWidget(undoButton.release());

    finishedSprintsView = new QListView;
    finishedSprintsView->setContextMenuPolicy(Qt::CustomContextMenu);
    finishedSprintsView->setModel(&sprintModel);
    connect(finishedSprintsView,
            &QListView::customContextMenuRequested,
            this,
            &SprintOutline::showContextMenu);
    layout->addWidget(finishedSprintsView);

    connect(addNewSprintButton.get(),
            &QPushButton::clicked,
            [&addSprintDialog]() { addSprintDialog.exec(); });
    layout->addWidget(addNewSprintButton.release());

    setLayout(layout);
}

SprintOutline::~SprintOutline() = default;

void SprintOutline::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu contextMenu;
    contextMenu.installEventFilter(new MouseRightReleaseEater(&contextMenu));
    const auto deleteEntry = "Delete";
    contextMenu.addAction(deleteEntry);

    QAction* selectedEntry = contextMenu.exec(globalPos);

    if (selectedEntry && selectedEntry->text() == deleteEntry) {
        QModelIndex index = finishedSprintsView->currentIndex();
        sprintModel.remove(index.row());
    }
}

} // namespace sprint_timer::ui::qt_gui
