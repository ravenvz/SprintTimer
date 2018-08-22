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
#ifndef TASKSPRINTSVIEW_H_HYCTEOV4
#define TASKSPRINTSVIEW_H_HYCTEOV4

#include "qt_gui/models/HistoryModel.h"
#include <QStyledItemDelegate>
#include <QWidget>
#include <core/entities/Sprint.h>
#include <memory>

namespace Ui {
class TaskSprintsView;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class TaskSprintsView : public QWidget {

    Q_OBJECT

public:
    TaskSprintsView(HistoryModel& model,
                    QStyledItemDelegate& delegate,
                    QWidget* parent = nullptr);

    explicit TaskSprintsView(QWidget* parent = nullptr);

    ~TaskSprintsView() override;

    void setData(const std::vector<entities::Sprint>& sprints);

private:
    std::unique_ptr<Ui::TaskSprintsView> ui;
    HistoryModel& model;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TASKSPRINTSVIEW_H_HYCTEOV4 */
