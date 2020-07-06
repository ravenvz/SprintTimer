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

#include "qt_gui/presentation/TodaySprints.h"
#include <QDialog>
#include <QListView>
#include <QPushButton>
#include <memory>

namespace sprint_timer::ui::qt_gui {

class SprintOutline : public contracts::TodaySprints::View, public QWidget {

public:
    SprintOutline(contracts::TodaySprints::Presenter& presenter,
                  std::unique_ptr<QWidget> undoWidget,
                  std::unique_ptr<QWidget> manualSprintAddWidget,
                  QWidget* parent = nullptr);

    ~SprintOutline() override;

    void displaySprints(const std::vector<entities::Sprint>& sprints) override;

    void displayAddSprintDialog(const std::vector<entities::Task>& activeTasks,
                                dw::Weekday firstDayOfWeek,
                                std::chrono::minutes sprintDuration) override;

private:
    contracts::TodaySprints::Presenter& presenter;
    QAbstractItemModel* sprintModel;

    void
    onSprintModelRowsAboutToBeRemoved(const QModelIndex&, int first, int last);
};

} // namespace sprint_timer::ui::qt_gui

#endif // SPRINT_TIMER_SPRINTOUTLINE_H
