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
#ifndef PLANNINGWINDOW_H_ICDEIZFK
#define PLANNINGWINDOW_H_ICDEIZFK

#include "qt_gui/widgets/StandaloneDisplayableWidget.h"

#include <QTreeView>
#include <QVBoxLayout>
#include <memory>
#include <qpushbutton.h>

#include <QStandardItemModel>

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"

#include <optional>
#include <span>

namespace sprint_timer::ui::contracts::PlannerContract {

struct TaskItemDTO {
    std::string name;
    std::string uuid;
    int level;
    std::vector<TaskItemDTO> subtasks;
    std::optional<std::string> parent;
};

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual void displayPlanner(std::span<TaskItemDTO> items) = 0;
};

} // namespace sprint_timer::ui::contracts::PlannerContract

namespace sprint_timer::ui::qt_gui {

class PlannerModel : public QStandardItemModel,
                     public contracts::PlannerContract::View {
public:
    explicit PlannerModel(QObject* parent = nullptr)
        : QStandardItemModel{parent}
    {
        using contracts::PlannerContract::TaskItemDTO;
        TaskItemDTO sub4{"Sub task 4", "5", 3, std::vector<TaskItemDTO>{}, "3"};
        TaskItemDTO sub3{"Sub task 3", "4", 3, std::vector<TaskItemDTO>{}, "3"};
        TaskItemDTO sub2{"Sub task 2", "3", 2, {sub3, sub4}, "2"};
        TaskItemDTO sub1{"Sub task 1", "2", 1, {sub2}, "1"};
        TaskItemDTO inboxItem1{
            "Some inbox task", "6", 1, std::vector<TaskItemDTO>{}, "0"};
        TaskItemDTO inboxItem2{
            "Some inbox task", "7", 1, std::vector<TaskItemDTO>{}, "0"};
        TaskItemDTO inboxRootItem{
            "Inbox", "0", 0, {inboxItem1, inboxItem2}, std::nullopt};
        TaskItemDTO rootItem{"Some project", "1", 0, {sub1}, std::nullopt};
        std::vector<TaskItemDTO> items{inboxItem1, rootItem};
        displayPlanner(items);
    }

    void displayPlanner(
        std::span<contracts::PlannerContract::TaskItemDTO> items) override
    {
        clear();
        auto rootItem = std::make_unique<QStandardItem>("Inbox");
        rootItem->setChild(
            0, std::make_unique<QStandardItem>("Some inbox task").release());
        rootItem->setChild(
            1,
            std::make_unique<QStandardItem>("Some other inbox task").release());
        auto treeRootItem = std::make_unique<QStandardItem>("Some project");
        auto subProject1 = std::make_unique<QStandardItem>("Sub task 1");
        auto subProject2 = std::make_unique<QStandardItem>("Sub task 2");
        auto subProject3 = std::make_unique<QStandardItem>("Sub task 3");
        auto subProject4 = std::make_unique<QStandardItem>("Sub task 4");
        subProject2->setChild(0, subProject3.release());
        subProject2->setChild(1, subProject4.release());
        subProject1->setChild(0, subProject2.release());
        treeRootItem->setChild(0, subProject1.release());
        appendRow(rootItem.release());
        appendRow(treeRootItem.release());
    }

    void displayGoals() { }

    void displayProjects() { }

    void displayReviews() { }
};

class PlanningWindow : public StandaloneDisplayableWidget {
public:
    explicit PlanningWindow(QWidget* parent = nullptr)
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

        outlineView->setModel(&plannerModel);
        std::vector<contracts::PlannerContract::TaskItemDTO> items;
        plannerModel.displayPlanner(items);
    }

private:
    PlannerModel plannerModel;
    QTreeView* outlineView = std::make_unique<QTreeView>().release();
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PLANNINGWINDOW_H_ICDEIZFK */

// def deserialize(self, s):
//         if len(s) == 0:
//             return
//         vals = s.split(",")
//         q = collections.deque()
//         root = Node(vals[0])
//         q.append(root)
//         i = 1
//         while q:
//             node = q.popleft()
//             i += 1
//             while vals[i] != "#":
//                 child = Node(vals[i])
//                 node.children.append(child)
//                 q.append(child)
//                 i += 1
//         return root
