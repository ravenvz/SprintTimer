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
#include "widgets/TaskSprintsView.h"
#include "ui_sprints_for_task_view.h"

namespace sprint_timer::ui::qt_gui {

TaskSprintsView::TaskSprintsView(QWidget* parent)
    : QWidget{parent}
    , ui{std::make_unique<Ui::TaskSprintsView>()}
{
    ui->setupUi(this);
    ui->treeView->setHeaderHidden(true);
}

TaskSprintsView::~TaskSprintsView() = default;

void TaskSprintsView::setModel(QStandardItemModel* model)
{
    ui->treeView->setModel(model);
    ui->treeView->expandAll();
}

void TaskSprintsView::setDelegate(QStyledItemDelegate* delegate)
{
    ui->treeView->setItemDelegate(delegate);
}

} // namespace sprint_timer::ui::qt_gui
