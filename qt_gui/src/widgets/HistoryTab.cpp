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
#include "qt_gui/widgets/HistoryTab.h"
#include <QTreeView>

namespace sprint_timer::ui::qt_gui {

HistoryTab::HistoryTab(contracts::HistoryContract::Presenter& presenter_)
    : presenter{presenter_}
{
    auto sprintWidget = std::make_unique<QTreeView>();
    sprintWidget->setItemDelegate(&delegate);
    sprintWidget->setHeaderHidden(true);
    addTab(sprintWidget.release(), "Sprints");

    auto taskWidget = std::make_unique<QTreeView>();
    taskWidget->setItemDelegate(&delegate);
    taskWidget->setHeaderHidden(true);
    addTab(taskWidget.release(), "Tasks");

    // presenter.onDisplayedTabChanged(currentIndex());

    connect(this, &QTabWidget::currentChanged, [this] {
        presenter.onDisplayedTabChanged(currentIndex());
    });

    show();

    presenter.attachView(*this);
}

HistoryTab::~HistoryTab() { presenter.detachView(*this); }

void HistoryTab::displayHistory(
    const contracts::HistoryContract::History& history)
{
    historyModel.fill(history);
    auto* v = dynamic_cast<QTreeView*>(widget(currentIndex()));
    v->setModel(&historyModel);
    v->expandAll();
}

contracts::HistoryContract::TaskEditData HistoryTab::openEditTaskDialog(
    const contracts::HistoryContract::TaskEditData& data)
{
    return contracts::HistoryContract::TaskEditData{};
}

contracts::HistoryContract::SprintEditData HistoryTab::openEditSprintDialog(
    const contracts::HistoryContract::SprintEditData& data)
{
    return contracts::HistoryContract::SprintEditData{
        dw::DateTimeRange{dw::current_date_time(), dw::current_date_time()}};
}

} // namespace sprint_timer::ui::qt_gui
