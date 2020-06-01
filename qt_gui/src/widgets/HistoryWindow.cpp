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
#include "qt_gui/widgets/HistoryWindow.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "qt_gui/widgets/DateRangePicker.h"
#include "ui_history.h"

namespace {

sprint_timer::ui::qt_gui::HistoryModel::HistoryItem convertItem(
    const sprint_timer::ui::contracts::HistoryContract::HistoryItem& item);

sprint_timer::ui::qt_gui::HistoryModel::HistoryData convertHistory(
    const std::vector<
        sprint_timer::ui::contracts::HistoryContract::HistoryItem>& history);

} // namespace

namespace sprint_timer::ui::qt_gui {

using namespace entities;

HistoryWindow::HistoryWindow(contracts::HistoryContract::Presenter& presenter_,
                             HistoryModel& historyModel_,
                             QStyledItemDelegate& historyItemDelegate_,
                             QWidget* parent_)
    : StandaloneDisplayableWidget{parent_}
    , ui{std::make_unique<Ui::HistoryWindow>()}
    , presenter{presenter_}
    , historyModel{historyModel_}
{
    ui->setupUi(this);
    ui->taskHistoryView->setHeaderHidden(true);
    ui->sprintHistoryView->setHeaderHidden(true);
    ui->sprintHistoryView->setItemDelegate(&historyItemDelegate_);
    ui->taskHistoryView->setItemDelegate(&historyItemDelegate_);

    connections.push_back(connect(ui->historyTab,
                                  &QTabWidget::currentChanged,
                                  this,
                                  &HistoryWindow::onTabSelected));
    connections.push_back(connect(ui->exportButton,
                                  &QPushButton::clicked,
                                  this,
                                  &HistoryWindow::onExportButtonClicked));
}

HistoryWindow::~HistoryWindow()
{
    std::for_each(begin(connections), end(connections), [](auto& connection) {
        disconnect(connection);
    });
}

void HistoryWindow::displaySprintHistory(
    const std::vector<contracts::HistoryContract::HistoryItem>& history)
{
    fillHistoryModel(convertHistory(history));
    setHistoryModel(ui->sprintHistoryView);
}

void HistoryWindow::displayTaskHistory(
    const std::vector<contracts::HistoryContract::HistoryItem>& history)
{
    fillHistoryModel(convertHistory(history));
    setHistoryModel(ui->taskHistoryView);
}

contracts::HistoryContract::TaskEditData HistoryWindow::openEditTaskDialog(
    const contracts::HistoryContract::TaskEditData& data)
{
    // TODO placeholder
    return contracts::HistoryContract::TaskEditData{};
}

contracts::HistoryContract::SprintEditData HistoryWindow::openEditSprintDialog(
    const contracts::HistoryContract::SprintEditData& data)
{
    // TODO placeholder
    return contracts::HistoryContract::SprintEditData{
        dw::DateTimeRange{dw::current_date_time(), dw::current_date_time()}};
}

void HistoryWindow::fillHistoryModel(const HistoryModel::HistoryData& history)
{
    historyModel.fill(history);
}

void HistoryWindow::onTabSelected(int tabIndex)
{
    presenter.onHistoryPaneChanged(tabIndex);
}

void HistoryWindow::setHistoryModel(QTreeView* view)
{
    view->setModel(&historyModel);
    view->expandAll();
    view->show();
}

void HistoryWindow::onExportButtonClicked() { }

} // namespace sprint_timer::ui::qt_gui

namespace {

sprint_timer::ui::qt_gui::HistoryModel::HistoryItem convertItem(
    const sprint_timer::ui::contracts::HistoryContract::HistoryItem& item)
{
    const auto& [date, description, uuid] = item;
    return sprint_timer::ui::qt_gui::HistoryModel::HistoryItem{
        sprint_timer::ui::qt_gui::utils::toQDate(date),
        QString::fromStdString(description)};
}

sprint_timer::ui::qt_gui::HistoryModel::HistoryData convertHistory(
    const std::vector<
        sprint_timer::ui::contracts::HistoryContract::HistoryItem>& history)
{
    sprint_timer::ui::qt_gui::HistoryModel::HistoryData convertedHistory;
    convertedHistory.reserve(history.size());
    std::transform(cbegin(history),
                   cend(history),
                   std::back_inserter(convertedHistory),
                   convertItem);
    return convertedHistory;
}

} // namespace
