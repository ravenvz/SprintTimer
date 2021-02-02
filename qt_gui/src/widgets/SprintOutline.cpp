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
#include "qt_gui/widgets/SprintOutline.h"
#include "qt_gui/dialogs/AddSprintDialog.h"
#include "qt_gui/metatypes/SprintMetaType.h"
#include "qt_gui/models/PomodoroModel.h"
#include <QVBoxLayout>
#include <qt_gui/widgets/ContextMenuListView.h>

namespace {

void refillModel(QAbstractItemModel* model,
                 const std::vector<sprint_timer::entities::Sprint>& sprints);

} // namespace

namespace sprint_timer::ui::qt_gui {

SprintOutline::SprintOutline(contracts::TodaySprints::Presenter& presenter_,
                             std::unique_ptr<QWidget> undoWidget_,
                             Displayable& addSprintDialog_,
                             QWidget* parent_)
    : QWidget{parent_}
    , presenter{presenter_}
{
    auto layout = std::make_unique<QVBoxLayout>();
    auto sprintView = std::make_unique<ContextMenuListView>(nullptr);
    auto addSprintButton = std::make_unique<QPushButton>("Add Sprint");
    sprintModel = std::make_unique<PomodoroModel>().release();
    sprintView->setModel(sprintModel);

    connect(addSprintButton.get(),
            &QPushButton::clicked,
            [&addSprintDialog_]() { addSprintDialog_.display(); });

    layout->addWidget(undoWidget_.release());

    layout->addWidget(sprintView.release());

    layout->addWidget(addSprintButton.release());

    setLayout(layout.release());
    presenter_.attachView(*this);
}

SprintOutline::~SprintOutline() { presenter.detachView(*this); }

void SprintOutline::displaySprints(const std::vector<entities::Sprint>& sprints)
{
    disconnect(sprintModel,
               &QAbstractItemModel::rowsAboutToBeRemoved,
               this,
               &SprintOutline::onSprintModelRowsAboutToBeRemoved);

    refillModel(sprintModel, sprints);

    connect(sprintModel,
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &SprintOutline::onSprintModelRowsAboutToBeRemoved);

} // namespace sprint_timer::ui::qt_gui

void SprintOutline::onSprintModelRowsAboutToBeRemoved(const QModelIndex&,
                                                      int first,
                                                      int last)
{
    for (int row = first; row <= last; ++row) {
        const auto var = sprintModel->data(sprintModel->index(row, 0),
                                           PomodoroModel::Roles::ItemRole);
        const auto sprint = var.value<entities::Sprint>();
        presenter.onSprintDelete(sprint);
    }
}

void SprintOutline::displayAddSprintDialog(
    const std::vector<entities::Task>& activeTasks,
    dw::Weekday firstDayOfWeek,
    std::chrono::minutes sprintDuration)
{
    std::vector<entities::Sprint> addedSprints;
    // AddSprintDialog dialog{
    //     activeTasks, addedSprints, firstDayOfWeek, sprintDuration};
    // dialog.setWindowModality(Qt::WindowModal);
    // dialog.exec();
}

} // namespace sprint_timer::ui::qt_gui

namespace {

void refillModel(QAbstractItemModel* model,
                 const std::vector<sprint_timer::entities::Sprint>& sprints)
{
    model->removeRows(0, model->rowCount());

    for (const auto& sprint : sprints) {
        QVariant var;
        var.setValue(sprint);
        model->insertRow(model->rowCount());
        model->setData(
            model->index(model->rowCount() - 1, 0),
            var,
            sprint_timer::ui::qt_gui::PomodoroModel::Roles::InsertRole);
    }
}

} // namespace
