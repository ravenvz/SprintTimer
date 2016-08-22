/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "historyview.h"
#include "ui_history.h"
#include <QListView>
#include <QPainter>
#include "utils/DateTimeConverter.h"


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

HistoryWindow::HistoryWindow(ICoreService& coreService, QWidget* parent)
    : DataWidget(parent)
    , ui(new Ui::HistoryWindow)
    , coreService{coreService}
    , displaySprintsState{std::make_unique<DiplaySprints>(*this)}
    , displayTasksState{std::make_unique<DisplayTasks>(*this)}
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    coreService.yearRange(std::bind(
            &HistoryWindow::onYearRangeUpdated, this, std::placeholders::_1));
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
            SIGNAL(timeSpanChanged(DateInterval)),
            this,
            SLOT(onDatePickerIntervalChanged(DateInterval)));
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

void HistoryWindow::onYearRangeUpdated(const std::vector<std::string>& yearRange)
{
    ui->dateRangePicker->setYears(yearRange);
}

void HistoryWindow::setHistoryModel(QTreeView* view)
{
    view->setModel(viewModel);
    view->expandAll();
    view->show();
}

DisplayState::DisplayState(HistoryWindow& historyView)
    : historyView{historyView}
{
}

DiplaySprints::DiplaySprints(HistoryWindow& historyView)
    : DisplayState{historyView}
{
}

DisplayTasks::DisplayTasks(HistoryWindow& historyView)
    : DisplayState{historyView}
{
}

void DiplaySprints::retrieveHistory()
{
    historyView.coreService.sprintsInTimeRange(
            historyView.selectedDateInterval.toTimeSpan(),
            std::bind(&DiplaySprints::onHistoryRetrieved,
                      this,
                      std::placeholders::_1));
}

void DisplayTasks::retrieveHistory()
{
    historyView.coreService.requestFinishedTasks(
        historyView.selectedDateInterval.toTimeSpan(),
        std::bind(&DisplayTasks::onHistoryRetrieved,
                  this,
                  std::placeholders::_1));
}

void DiplaySprints::onHistoryRetrieved(
    const std::vector<Sprint>& sprints)
{
    std::vector<HistoryWindow::HistoryItem> sprintHistory;
    sprintHistory.reserve(sprints.size());
    std::transform(sprints.cbegin(),
                   sprints.cend(),
                   std::back_inserter(sprintHistory),
                   [](const auto& sprint) {
                       return std::make_pair(
                           DateTimeConverter::qDate(sprint.startTime()),
                           QString::fromStdString(sprint.toString()));
                   });
    historyView.fillHistoryModel(sprintHistory);
    historyView.setHistoryModel(historyView.ui->lvSprintHistory);
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
                           QString::fromStdString(task.toString()));
                   });

    historyView.fillHistoryModel(taskHistory);
    historyView.setHistoryModel(historyView.ui->lvTaskHistory);
}
