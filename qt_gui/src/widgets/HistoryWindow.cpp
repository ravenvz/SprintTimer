/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#include "HistoryWindow.h"
#include "ui_history.h"
#include <QPainter>
#include <fstream>
#include "core/utils/CSVEncoder.h"
#include "core/external_io/OstreamSink.h"

#include <QDebug>

HistoryViewDelegate::HistoryViewDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}


void HistoryViewDelegate::paint(QPainter* painter,
                                const QStyleOptionViewItem& option,
                                const QModelIndex& index) const
{
    if (!index.parent().isValid()) {
        QStyleOptionViewItem changedOption{option};
        changedOption.font.setWeight(QFont::Bold);
        QStyledItemDelegate::paint(painter, changedOption, index);
    }
    else {
        QStyledItemDelegate::paint(painter, option, index);
    }
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
}


HistoryWindow::HistoryWindow(ICoreService& coreService, QWidget* parent)
    : DataWidget(parent)
    , ui(new Ui::HistoryWindow)
    , coreService{coreService}
    , displaySprintsState{std::make_unique<DisplaySprints>(*this)}
    , displayTasksState{std::make_unique<DisplayTasks>(*this)}
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    coreService.yearRange([this](const auto& range) { this->onYearRangeUpdated(range); });
    selectedDateInterval = ui->dateRangePicker->getInterval();
    viewModel = new QStandardItemModel(this);
    ui->lvTaskHistory->setHeaderHidden(true);
    ui->lvSprintHistory->setHeaderHidden(true);
    ui->lvSprintHistory->setItemDelegate(new HistoryViewDelegate(this));
    ui->lvTaskHistory->setItemDelegate(new HistoryViewDelegate(this));
    historyState = displayTasksState.get();
    connect(ui->twHistoryDisplay,
            &QTabWidget::currentChanged,
            this,
            &HistoryWindow::onTabSelected);
    connect(ui->dateRangePicker,
            &DateRangePicker::timeSpanChanged,
            this,
            &HistoryWindow::onDatePickerIntervalChanged);
    connect(ui->pbExport,
            &QPushButton::clicked,
            this,
            &HistoryWindow::onExportButtonClicked);
}


HistoryWindow::~HistoryWindow() { delete ui; }


void HistoryWindow::synchronize() { historyState->retrieveHistory(); }


void HistoryWindow::fillHistoryModel(const std::vector<HistoryItem>& history)
{
    // QStandardItemModel takes ownership of items that are added with
    // appendRow()
    // so they will be deleted when model is deleted.
    viewModel->clear();
    if (history.empty()) {
        viewModel->appendRow(new QStandardItem("No data for selected period."));
        return;
    }
    QStandardItem* parent
        = new QStandardItem(QString("Completed %1 items.").arg(history.size()));
    viewModel->appendRow(parent);
    int children{0};
    QDate date = history.front().first;
    parent = new QStandardItem(date.toString());
    viewModel->appendRow(parent);
    for (const auto& historyItem : history) {
        if (historyItem.first != date) {
            children = 0;
            date = historyItem.first;
            parent = new QStandardItem(date.toString());
            viewModel->appendRow(parent);
        }
        QStandardItem* item = new QStandardItem(historyItem.second);
        parent->setChild(children, item);
        ++children;
    }
}


void HistoryWindow::onDatePickerIntervalChanged(DateInterval newInterval)
{
    selectedDateInterval = newInterval;
    synchronize();
}


void HistoryWindow::onTabSelected(int tabIndex)
{
    if (tabIndex == sprintTabIndex) {
        historyState = displaySprintsState.get();
    }
    if (tabIndex == taskTabIndex) {
        historyState = displayTasksState.get();
    }
    synchronize();
}


void HistoryWindow::onYearRangeUpdated(
    const std::vector<std::string>& yearRange)
{
    ui->dateRangePicker->setYears(yearRange);
}


void HistoryWindow::setHistoryModel(QTreeView* view)
{
    view->setModel(viewModel);
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
    using namespace ExternalIO;
    auto filePath = ss.str();
    auto out = std::make_shared<std::ofstream>(filePath);
    auto sink = std::make_shared<OstreamSink>(out);
    auto serializeSprint = [](const Sprint& sprint) {
        std::vector<std::string> str;
        auto tags = sprint.tags();
        str.emplace_back(StringUtils::join(cbegin(tags), cend(tags), ", "));
        str.emplace_back(sprint.timeSpan().toString("hh:MM"));
        str.emplace_back(sprint.name());
        str.emplace_back(sprint.taskUuid());
        str.emplace_back(sprint.uuid());
        return str;
    };
    auto func = [serializeSprint](const auto& sprints) {
        CSV::CSVEncoder encoder;
        return encoder.encode(sprints, serializeSprint);
    };
    historyView.coreService.exportSprints(
            historyView.selectedDateInterval.toTimeSpan(),
            sink,
            func);
}


void DisplaySprints::onHistoryRetrieved(const std::vector<Sprint>& sprints)
{
    std::vector<HistoryWindow::HistoryItem> sprintHistory;
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
    historyView.setHistoryModel(historyView.ui->lvSprintHistory);
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
    using namespace ExternalIO;
    auto filePath = ss.str();
    auto out = std::make_shared<std::ofstream>(filePath);
    auto sink = std::make_shared<OstreamSink>(out);
    auto serializeTask = [](const Task& task) {
        std::vector<std::string> str;
        auto tags = task.tags();
        str.emplace_back(StringUtils::join(cbegin(tags), cend(tags), ", "));
        str.emplace_back(task.name());
        str.emplace_back(task.uuid());
        str.emplace_back(std::to_string(task.isCompleted()));
        str.emplace_back(std::to_string(task.actualCost()));
        str.emplace_back(std::to_string(task.estimatedCost()));
        return str;
    };
    auto func = [serializeTask](const auto& tasks) {
        CSV::CSVEncoder encoder;
        return encoder.encode(tasks, serializeTask);
    };
    historyView.coreService.exportTasks(
            historyView.selectedDateInterval.toTimeSpan(),
            sink,
            func);
}


void DisplayTasks::onHistoryRetrieved(const std::vector<Task>& tasks)
{
    std::vector<HistoryWindow::HistoryItem> taskHistory;
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
    historyView.setHistoryModel(historyView.ui->lvTaskHistory);
}
