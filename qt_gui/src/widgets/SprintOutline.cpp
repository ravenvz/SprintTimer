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

#include "widgets/SprintOutline.h"
#include "dialogs/AddSprintDialog.h"
#include "ui_sprint_outline.h"
#include "dialogs/ConfirmationDialog.h"
#include <QMenu>

SprintOutline::SprintOutline(ICoreService& coreService,
                             IConfig& applicationSettings,
                             SprintModel* sprintModel,
                             TaskModel* taskModel,
                             QWidget* parent)
    : ui{new Ui::SprintOutline}
    , QWidget{parent}
    , coreService{coreService}
    , applicationSettings{applicationSettings}
    , sprintModel{sprintModel}
    , taskModel{taskModel}
{
    ui->setupUi(this);
    ui->lvFinishedSprints->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->lvFinishedSprints->setModel(sprintModel);

    connect(ui->pbAddSprintManually,
            &QPushButton::clicked,
            this,
            &SprintOutline::launchManualAddSprintDialog);
    // Disables AddSprint button when there are no active tasks.
    connect(taskModel,
            &QAbstractListModel::modelReset,
            this,
            &SprintOutline::adjustAddSprintButtonState);
    connect(ui->lvFinishedSprints,
            &QListView::customContextMenuRequested,
            this,
            &SprintOutline::showContextMenu);
}

void SprintOutline::launchManualAddSprintDialog()
{
    AddSprintDialog dialog{
            sprintModel, taskModel, applicationSettings.sprintDuration()};
    dialog.exec();
}

void SprintOutline::adjustAddSprintButtonState()
{
    ui->pbAddSprintManually->setEnabled(taskModel->rowCount() != 0);
}

QSize SprintOutline::sizeHint() const
{
    return desiredSize;
}

void SprintOutline::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu contextMenu;
    const auto deleteEntry = "Delete";
    contextMenu.addAction(deleteEntry);

    QAction* selectedEntry = contextMenu.exec(globalPos);

    if (selectedEntry && selectedEntry->text() == deleteEntry)
        removeSprint();
}

void SprintOutline::removeSprint()
{
    QModelIndex index = ui->lvFinishedSprints->currentIndex();
    ConfirmationDialog dialog;
    QString description{"Remove sprint?"};
    dialog.setActionDescription(description);
    if (dialog.exec())
        sprintModel->remove(index.row());
}

