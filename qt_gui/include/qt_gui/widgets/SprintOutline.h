/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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

#include <QListView>
#include <QPushButton>
#include <QWidget>
#include <memory>

namespace sprint_timer::ui::qt_gui {

class SprintModel;
class AddSprintDialog;
class UndoDialog;
class SprintView;

class SprintOutline : public QWidget {

    Q_OBJECT

public:
    SprintOutline(AddSprintDialog& addSprintDialog,
                  UndoDialog& undoDialog,
                  std::unique_ptr<QPushButton> undoButton,
                  std::unique_ptr<QPushButton> addNewSprintButton,
                  std::unique_ptr<QListView> sprintView,
                  QWidget* parent = nullptr);

    ~SprintOutline() override;
};

} // namespace sprint_timer::ui::qt_gui


#endif // SPRINT_TIMER_SPRINTOUTLINE_H
