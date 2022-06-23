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

#include "qt_gui/widgets/StandaloneDisplayableWidget.h"
#include <QAbstractItemModel>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>

namespace sprint_timer::ui::qt_gui {

class PlannerWindow : public StandaloneDisplayableWidget {
public:
    explicit PlannerWindow(std::unique_ptr<QAbstractItemModel> plannerModel_,
                           QWidget* parent_ = nullptr)

        : StandaloneDisplayableWidget{parent_}
        , plannerModel{std::move(plannerModel_)}
    {
        auto lt = std::make_unique<QHBoxLayout>();

        auto pbPlanner = std::make_unique<QPushButton>("Planning");
        auto pbGoals = std::make_unique<QPushButton>("Goals");
        auto pbProjects = std::make_unique<QPushButton>("Projects");
        auto pbReview = std::make_unique<QPushButton>("Review");
        auto buttonLayout = std::make_unique<QVBoxLayout>();

        auto buttonWidget = std::make_unique<QWidget>();
        buttonLayout->addWidget(pbPlanner.release());
        buttonLayout->addWidget(pbGoals.release());
        buttonLayout->addWidget(pbProjects.release());
        buttonLayout->addWidget(pbReview.release());
        buttonWidget->setLayout(buttonLayout.release());
        lt->addWidget(buttonWidget.release());
        lt->addWidget(outlineView);

        setLayout(lt.release());

        outlineView->setModel(plannerModel.get());
        // const auto items = buildSampleTree();
        // plannerModel.displayPlanner(items);
    }

private:
    std::unique_ptr<QAbstractItemModel> plannerModel;
    QTreeView* outlineView = std::make_unique<QTreeView>().release();
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PLANNERWINDOW_H_SRA2PAHG */
