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
#include "qt_gui/widgets/PlannerWindow.h"
#include "cpp_utils/algorithms/optional_ext.h"
#include "cpp_utils/algorithms/string_ext.h"
#include "qt_gui/models/CustomRoles.h"
#include "qt_gui/models/PlannerModel.h"
#include "qt_gui/widgets/ReordableTreeView.h"
#include <QAbstractItemModel>
#include <QDialog>
#include <QHeaderView>
#include <QMenu>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>
#include <unordered_set>

namespace {

constexpr int plannerViewsStretch{1};
constexpr int plannerTreeStretch{4};

} // namespace

namespace sprint_timer::ui::qt_gui {

PlannerWindow::PlannerWindow(std::unique_ptr<QWidget> plannerViews_,
                             std::unique_ptr<QWidget> plannerTreeWidget_,
                             QWidget* parent_)
    : StandaloneDisplayableWidget{parent_}
{
    auto lt = std::make_unique<QHBoxLayout>();

    lt->addWidget(plannerViews_.release(), plannerViewsStretch);
    lt->addWidget(plannerTreeWidget_.release(), plannerTreeStretch);

    setLayout(lt.release());

    setMinimumWidth(1280);
    setMinimumHeight(1024);
}

} // namespace sprint_timer::ui::qt_gui

