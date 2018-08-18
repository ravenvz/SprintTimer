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
#include "qt_gui/dialogs/ConfirmationDialog.h"
#include "qt_gui/utils/MouseRightReleaseEater.h"
#include "ui_sprint_outline.h"
#include <QMenu>

namespace sprint_timer::ui::qt_gui {

SprintOutline::SprintOutline(IConfig& applicationSettings,
                             SprintModel& sprintModel,
                             TaskModel& taskModel,
                             CommandInvoker& commandInvoker,
                             QWidget* parent)
    : QWidget{parent}
    , ui{std::make_unique<Ui::SprintOutline>()}
    , applicationSettings{applicationSettings}
    , sprintModel{sprintModel}
    , taskModel{taskModel}
    , commandInvoker{commandInvoker}
{
    ui->setupUi(this);
    ui->lvFinishedSprints->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->lvFinishedSprints->setModel(&sprintModel);

    commandInvoker.attach(*this);

    connect(ui->pbAddSprintManually,
            &QPushButton::clicked,
            this,
            &SprintOutline::launchManualAddSprintDialog);
    // Disables AddSprint button when there are no active tasks.
    connect(&taskModel,
            &QAbstractListModel::modelReset,
            this,
            &SprintOutline::adjustAddSprintButtonState);
    connect(ui->lvFinishedSprints,
            &QListView::customContextMenuRequested,
            this,
            &SprintOutline::showContextMenu);
    connect(ui->pbUndo,
            &QPushButton::clicked,
            this,
            &SprintOutline::onUndoButtonClicked);

    adjustUndoButtonState();
}

SprintOutline::~SprintOutline() = default;

void SprintOutline::launchManualAddSprintDialog()
{
    addSprintDialog.reset(new AddSprintDialog{
        sprintModel, taskModel, applicationSettings.sprintDuration()});
    addSprintDialog->setModal(true);
    addSprintDialog->show();
}

void SprintOutline::adjustAddSprintButtonState()
{
    ui->pbAddSprintManually->setEnabled(taskModel.rowCount(QModelIndex()) != 0);
}

QSize SprintOutline::sizeHint() const { return desiredSize; }

void SprintOutline::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu contextMenu;
    contextMenu.installEventFilter(new MouseRightReleaseEater(&contextMenu));
    const auto deleteEntry = "Delete";
    contextMenu.addAction(deleteEntry);

    QAction* selectedEntry = contextMenu.exec(globalPos);

    if (selectedEntry && selectedEntry->text() == deleteEntry) {
        QModelIndex index = ui->lvFinishedSprints->currentIndex();
        sprintModel.remove(index.row());
    }
}

void SprintOutline::onUndoButtonClicked()
{
    ConfirmationDialog dialog;
    QString description{"Revert following action:\n"};
    description.append(
        QString::fromStdString(commandInvoker.lastCommandDescription()));
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        commandInvoker.undo();
        sprintModel.synchronize();
        taskModel.synchronize();
        emit actionUndone();
    }
}

void SprintOutline::update() { adjustUndoButtonState(); }

void SprintOutline::adjustUndoButtonState()
{
    ui->pbUndo->setEnabled(commandInvoker.hasUndoableCommands());
}

} // namespace sprint_timer::ui::qt_gui
