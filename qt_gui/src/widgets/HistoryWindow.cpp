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
#include "qt_gui/widgets/HistoryWindow.h"
#include "ui_history.h"
#include <QPainter>
#include <core/external_io/OstreamSink.h>
#include <core/utils/CSVEncoder.h>
#include <fstream>

namespace sprint_timer::ui::qt_gui {

using namespace entities;

namespace {

    QString sprintToString(const Sprint& sprint);

    QString taskToString(const Task& task);

} // namespace


HistoryWindow::HistoryWindow(ICoreService& coreService, QWidget* parent)
    : DataWidget{parent}
    , ui{std::make_unique<Ui::HistoryWindow>()}
    , coreService{coreService}
    , displaySprintsState{std::make_unique<DisplaySprints>(*this)}
    , displayTasksState{std::make_unique<DisplayTasks>(*this)}
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    coreService.yearRange(
        [this](const auto& range) { this->onYearRangeUpdated(range); });
    selectedDateInterval = ui->dateRangePicker->getInterval();
    ui->taskHistoryView->setHeaderHidden(true);
    ui->sprintHistoryView->setHeaderHidden(true);
    ui->sprintHistoryView->setItemDelegate(historyItemDelegate.get());
    ui->taskHistoryView->setItemDelegate(historyItemDelegate.get());
    historyState = displaySprintsState.get();

    connect(ui->historyTab,
            &QTabWidget::currentChanged,
            this,
            &HistoryWindow::onTabSelected);
    connect(ui->dateRangePicker,
            &DateRangePicker::timeSpanChanged,
            this,
            &HistoryWindow::onDatePickerIntervalChanged);
    connect(ui->exportButton,
            &QPushButton::clicked,
            this,
            &HistoryWindow::onExportButtonClicked);
}


HistoryWindow::~HistoryWindow() = default;


void HistoryWindow::synchronize() { historyState->retrieveHistory(); }


void HistoryWindow::fillHistoryModel(const HistoryModel::HistoryData& history)
{
    viewModel->fill(history);
}


void HistoryWindow::onDatePickerIntervalChanged(DateInterval newInterval)
{
    selectedDateInterval = newInterval;
    synchronize();
}


void HistoryWindow::onTabSelected(int tabIndex)
{
    if (tabIndex == sprintTabIndex)
        historyState = displaySprintsState.get();
    if (tabIndex == taskTabIndex)
        historyState = displayTasksState.get();
    synchronize();
}


void HistoryWindow::onYearRangeUpdated(
    const std::vector<std::string>& yearRange)
{
    ui->dateRangePicker->setYears(yearRange);
}


void HistoryWindow::setHistoryModel(QTreeView* view)
{
    view->setModel(viewModel.get());
    view->expandAll();
    view->show();
}


void HistoryWindow::onExportButtonClicked()
{
    exportDialog = std::make_unique<ExportDialog>();
    exportDialog->setModal(true);
    connect(exportDialog.get(),
            &ExportDialog::exportConfirmed,
            this,
            &HistoryWindow::onDataExportConfirmed);
    exportDialog->show();
}


void HistoryWindow::onDataExportConfirmed(
    const ExportDialog::ExportOptions& options)
{
    historyState->exportData(options);
}


DisplayState::DisplayState(HistoryWindow& historyView)
    : historyView{historyView}
{
}


DisplaySprints::DisplaySprints(HistoryWindow& historyView)
    : DisplayState{historyView}
{
}


DisplayTasks::DisplayTasks(HistoryWindow& historyView)
    : DisplayState{historyView}
{
}


void DisplaySprints::retrieveHistory()
{
    historyView.coreService.sprintsInTimeRange(
        historyView.selectedDateInterval.toTimeSpan(),
        [this](const auto& sprints) { this->onHistoryRetrieved(sprints); });
}


void DisplaySprints::exportData(const ExportDialog::ExportOptions& options)
{
    std::stringstream ss;
    ss << options.path << "/Sprints "
       << historyView.selectedDateInterval.toTimeSpan().toString("dd.MM.yyyy")
       << ".csv";
    using namespace external_io;
    auto filePath = ss.str();
    auto out = std::make_shared<std::ofstream>(filePath);
    auto sink = std::make_shared<OstreamSink>(out);
    auto serializeSprint = [](const Sprint& sprint) {
        std::vector<std::string> str;
        auto tags = sprint.tags();
        str.emplace_back(utils::join(cbegin(tags), cend(tags), ", "));
        str.emplace_back(sprint.timeSpan().toString("hh:MM"));
        str.emplace_back(sprint.name());
        str.emplace_back(sprint.taskUuid());
        str.emplace_back(sprint.uuid());
        return str;
    };
    auto func = [serializeSprint](const auto& sprints) {
        utils::CSVEncoder encoder;
        return encoder.encode(sprints, serializeSprint);
    };
    historyView.coreService.exportSprints(
        historyView.selectedDateInterval.toTimeSpan(), sink, func);
}


void DisplaySprints::onHistoryRetrieved(const std::vector<Sprint>& sprints)
{
    HistoryModel::HistoryData sprintHistory;
    sprintHistory.reserve(sprints.size());
    std::transform(sprints.cbegin(),
                   sprints.cend(),
                   std::back_inserter(sprintHistory),
                   [](const auto& sprint) {
                       return std::make_pair(
                           DateTimeConverter::qDate(sprint.startTime()),
                           sprintToString(sprint));
                   });
    historyView.fillHistoryModel(sprintHistory);
    historyView.setHistoryModel(historyView.ui->sprintHistoryView);
}


void DisplayTasks::retrieveHistory()
{
    historyView.coreService.requestFinishedTasks(
        historyView.selectedDateInterval.toTimeSpan(),
        [this](const auto& tasks) { this->onHistoryRetrieved(tasks); });
}


void DisplayTasks::exportData(const ExportDialog::ExportOptions& options)
{
    std::stringstream ss;
    ss << options.path << "/Tasks "
       << historyView.selectedDateInterval.toTimeSpan().toString("dd.MM.yyyy")
       << ".csv";
    using namespace external_io;
    auto filePath = ss.str();
    auto out = std::make_shared<std::ofstream>(filePath);
    auto sink = std::make_shared<OstreamSink>(out);
    auto serializeTask = [](const Task& task) {
        std::vector<std::string> str;
        auto tags = task.tags();
        str.emplace_back(utils::join(cbegin(tags), cend(tags), ", "));
        str.emplace_back(task.name());
        str.emplace_back(task.uuid());
        str.emplace_back(std::to_string(task.isCompleted()));
        str.emplace_back(std::to_string(task.actualCost()));
        str.emplace_back(std::to_string(task.estimatedCost()));
        return str;
    };
    auto func = [serializeTask](const auto& tasks) {
        utils::CSVEncoder encoder;
        return encoder.encode(tasks, serializeTask);
    };
    historyView.coreService.exportTasks(
        historyView.selectedDateInterval.toTimeSpan(), sink, func);
}


void DisplayTasks::onHistoryRetrieved(const std::vector<Task>& tasks)
{
    HistoryModel::HistoryData taskHistory;
    taskHistory.reserve(tasks.size());
    std::transform(tasks.cbegin(),
                   tasks.cend(),
                   std::back_inserter(taskHistory),
                   [](const auto& task) {
                       return std::make_pair(
                           DateTimeConverter::qDate(task.lastModified()),
                           taskToString(task));
                   });

    historyView.fillHistoryModel(taskHistory);
    historyView.setHistoryModel(historyView.ui->taskHistoryView);
}

namespace {

    QString sprintToString(const Sprint& sprint)
    {
        return QString("%1 - %2 %3 %4")
            .arg(QString::fromStdString(sprint.startTime().toString("hh:mm")))
            .arg(QString::fromStdString(sprint.finishTime().toString("hh:mm")))
            .arg(QString::fromStdString(prefixTags(sprint.tags())))
            .arg(QString::fromStdString(sprint.name()));
    }


    QString taskToString(const Task& task)
    {
        return QString("%1 %2 %3/%4")
            .arg(QString::fromStdString(prefixTags(task.tags())))
            .arg(QString::fromStdString(task.name()))
            .arg(task.actualCost())
            .arg(task.estimatedCost());
    }

} // namespace

} // namespace sprint_timer::ui::qt_gui
