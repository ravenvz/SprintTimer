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
#ifndef PLANNERWINDOW_H_SRA2PAHG
#define PLANNERWINDOW_H_SRA2PAHG

#include "qt_gui/presentation/PlannerContract.h"
#include "qt_gui/widgets/StandaloneDisplayableWidget.h"
#include <QAbstractItemDelegate>
#include <memory>

class QTreeView;

class QVBoxLayout;

class QAbstractItemModel;

namespace sprint_timer::ui::qt_gui {

class PlannerWindow : public StandaloneDisplayableWidget,
                      public contracts::PlannerContract::View {
public:
    PlannerWindow(QAbstractItemModel& plannerModel_,
                  QAbstractItemDelegate& itemDelegate_,
                  Displayable& addTaskDialog_,
                  Displayable& editTaskDialog_,
                  QWidget* parent_ = nullptr);

    auto displayPlanner(const contracts::PlannerContract::PlannerTree& taskTree)
        -> void override;

private:
    QAbstractItemModel& plannerModel;
    QTreeView* outlineView;
    Displayable& addTaskDialog;
    Displayable& editTaskDialog;

    auto showContextMenu(const QPoint& pos) const -> void;

    auto handleEdit(const QModelIndex& index) const -> void;

    auto handleToggle(const QModelIndex& index) const -> void;

    [[nodiscard]] auto selectedTaskUuid() const -> std::string;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PLANNERWINDOW_H_SRA2PAHG */
