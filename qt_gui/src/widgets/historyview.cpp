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

HistoryView::HistoryView(ICoreService& pomodoroService, QWidget* parent)
    : DataWidget(parent)
    , ui(new Ui::HistoryView)
    , pomodoroService{pomodoroService}
    , historyStatePomodoro{std::make_unique<HistoryStatePomodoro>(*this)}
    , historyStateTask{std::make_unique<HistoryStateTask>(*this)}
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    pomodoroService.yearRange(std::bind(
            &HistoryView::onYearRangeUpdated, this, std::placeholders::_1));
    selectedDateInterval = ui->widgetPickPeriod->getInterval();
    viewModel = new QStandardItemModel(this);
    ui->lvTodoHistory->setHeaderHidden(true);
    ui->lvPomodoroHistory->setHeaderHidden(true);
    ui->lvPomodoroHistory->setItemDelegate(new HistoryViewDelegate(this));
    ui->lvTodoHistory->setItemDelegate(new HistoryViewDelegate(this));
    historyState = historyStateTask.get();
    connect(ui->twHistoryDisplay,
            &QTabWidget::currentChanged,
            this,
            &HistoryView::onTabSelected);
    connect(ui->widgetPickPeriod,
            SIGNAL(timeSpanChanged(DateInterval)),
            this,
            SLOT(onDatePickerIntervalChanged(DateInterval)));
}

HistoryView::~HistoryView() { delete ui; }

void HistoryView::synchronize() { historyState->retrieveHistory(); }

void HistoryView::fillHistoryModel(const std::vector<HistoryItem>& history)
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

void HistoryView::onDatePickerIntervalChanged(DateInterval newInterval)
{
    selectedDateInterval = newInterval;
    synchronize();
}

void HistoryView::onTabSelected(int tabIndex)
{
    if (tabIndex == pomodoroTabIndex) {
        historyState = historyStatePomodoro.get();
    }
    if (tabIndex == taskTabIndex) {
        historyState = historyStateTask.get();
    }
    synchronize();
}

void HistoryView::onYearRangeUpdated(const std::vector<std::string>& yearRange)
{
    ui->widgetPickPeriod->setYears(yearRange);
}

void HistoryView::setHistoryModel(QTreeView* view)
{
    view->setModel(viewModel);
    view->expandAll();
    view->show();
}

HistoryState::HistoryState(HistoryView& historyView)
    : historyView{historyView}
{
}

HistoryStatePomodoro::HistoryStatePomodoro(HistoryView& historyView)
    : HistoryState{historyView}
{
}

HistoryStateTask::HistoryStateTask(HistoryView& historyView)
    : HistoryState{historyView}
{
}

void HistoryStatePomodoro::retrieveHistory()
{
    historyView.pomodoroService.sprintsInTimeRange(
            historyView.selectedDateInterval.toTimeSpan(),
            std::bind(&HistoryStatePomodoro::onHistoryRetrieved,
                      this,
                      std::placeholders::_1));
}

void HistoryStateTask::retrieveHistory()
{
    historyView.pomodoroService.requestFinishedTasks(
        historyView.selectedDateInterval.toTimeSpan(),
        std::bind(&HistoryStateTask::onHistoryRetrieved,
                  this,
                  std::placeholders::_1));
}

void HistoryStatePomodoro::onHistoryRetrieved(
    const std::vector<Sprint>& pomodoros)
{
    std::vector<HistoryView::HistoryItem> pomodoroHistory;
    pomodoroHistory.reserve(pomodoros.size());
    std::transform(pomodoros.cbegin(),
                   pomodoros.cend(),
                   std::back_inserter(pomodoroHistory),
                   [](const auto& pomo) {
                       return std::make_pair(
                           DateTimeConverter::qDate(pomo.startTime()),
                           QString::fromStdString(pomo.toString()));
                   });
    historyView.fillHistoryModel(pomodoroHistory);
    historyView.setHistoryModel(historyView.ui->lvPomodoroHistory);
}

void HistoryStateTask::onHistoryRetrieved(const std::vector<Task>& tasks)
{
    std::vector<HistoryView::HistoryItem> taskHistory;
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
    historyView.setHistoryModel(historyView.ui->lvTodoHistory);
}
