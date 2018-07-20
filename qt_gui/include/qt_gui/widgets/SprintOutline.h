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
#ifndef SPRINT_TIMER_SPRINTOUTLINE_H
#define SPRINT_TIMER_SPRINTOUTLINE_H

#include "qt_gui/dialogs/AddSprintDialog.h"
#include "qt_gui/models/SprintModel.h"
#include "qt_gui/models/TaskModel.h"
#include <QPushButton>
#include <QWidget>
#include <core/IConfig.h>
#include <core/ICoreService.h>
#include <core/Observer.h>
#include <memory>

namespace Ui {
class SprintOutline;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class SprintOutline : public QWidget, public Observer {

    Q_OBJECT

public:
    SprintOutline(ICoreService& coreService,
                  IConfig& applicationSettings,
                  SprintModel& sprintModel,
                  TaskModel& taskModel,
                  QWidget* parent);

    ~SprintOutline() override;

    QSize sizeHint() const override;

    void update() override;

private:
    Ui::SprintOutline* ui;
    ICoreService& coreService;
    IConfig& applicationSettings;
    SprintModel& sprintModel;
    TaskModel& taskModel;
    const QSize desiredSize{300, 200};
    std::unique_ptr<AddSprintDialog> addSprintDialog;

    void adjustUndoButtonState();

private slots:
    void launchManualAddSprintDialog();
    void adjustAddSprintButtonState();
    void showContextMenu(const QPoint& pos);
    void onUndoButtonClicked();

signals:
    void actionUndone();
};

} // namespace sprint_timer::ui::qt_gui


#endif // SPRINT_TIMER_SPRINTOUTLINE_H
