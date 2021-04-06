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
#ifndef TASKOUTLINE_H
#define TASKOUTLINE_H

#include "qt_gui/presentation/AddTaskControl.h"
#include "qt_gui/widgets/TagEditor.h"
#include <QWidget>
#include <memory>

class QLineEdit;
class Displayable;

namespace sprint_timer::ui::qt_gui {

class TaskOutline : public QWidget, public ui::contracts::AddTaskControl::View {
public:
    TaskOutline(std::unique_ptr<QWidget> taskView,
                Displayable& addTaskDialog,
                QWidget* parent = nullptr);

private:
    Displayable& addTaskDialog;
    QLineEdit* quickAddTask;

    void onAddTaskButtonPushed();

    void onQuickAddTodoReturnPressed();
};

} // namespace sprint_timer::ui::qt_gui

#endif // TASKOUTLINE_H
