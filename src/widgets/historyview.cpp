#include "historyview.h"
#include "core/use_cases/RequestPomodorosInTimeRangeCommand.h"
#include "db_layer/db_service.h"
#include "qt_storage_impl/QtPomoStorageReader.h"
#include "ui_history.h"
#include <QPainter>

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

HistoryView::HistoryView(DBService& dbService, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::HistoryView)
    , pomodoroStorageReader{std::make_unique<QtPomoStorageReader>(dbService)}
    , historyStatePomodoro{std::make_unique<HistoryStatePomodoro>(*this)}
    , historyStateTask{std::make_unique<HistoryStateTask>(*this)}
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    pomodoroModel = new PomodoroModel(this);
    todoItemModel = new TodoItemModel(this);
    ui->widgetPickPeriod->setYears(pomodoroModel->yearRange());
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
    updateView();
}

HistoryView::~HistoryView() { delete ui; }

void HistoryView::updateView() { historyState->retrieveHistory(); }

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
    updateView();
}

void HistoryView::onTabSelected(int tabIndex)
{
    if (tabIndex == pomodoroTabIndex) {
        historyState = historyStatePomodoro.get();
    }
    if (tabIndex == taskTabIndex) {
        historyState = historyStateTask.get();
    }
    updateView();
}


HistoryState::HistoryState(HistoryView& historyView)
    : historyView{historyView}
{
}

HistoryStatePomodoro::HistoryStatePomodoro(HistoryView& historyView)
    : HistoryState{historyView}
{
}

void HistoryStatePomodoro::retrieveHistory()
{
    UseCases::RequestPomodorosInTimeRangeCommand requestPomodoros{
        *historyView.pomodoroStorageReader,
        historyView.selectedDateInterval.toTimeSpan(),
        std::bind(&HistoryStatePomodoro::onHistoryRetrieved,
                  this,
                  std::placeholders::_1)};
    requestPomodoros.execute();
}


void HistoryStatePomodoro::onHistoryRetrieved(
    const std::vector<Pomodoro>& pomodoros)
{
    // std::vector<Pomodoro> pomodoros{historyView.pomodoroModel->items()};
    std::vector<HistoryView::HistoryItem> pomodoroHistory;
    pomodoroHistory.reserve(pomodoros.size());
    std::transform(
        pomodoros.cbegin(),
        pomodoros.cend(),
        std::back_inserter(pomodoroHistory),
        [](const auto& pomo) {
            return std::make_pair(
                QDateTime::fromTime_t(
                    static_cast<unsigned>(pomo.startTime().toTime_t()))
                    .date(),
                QString::fromStdString(pomo.toString()));
        });
    historyView.fillHistoryModel(pomodoroHistory);
    setHistoryModel();
}

void HistoryStatePomodoro::setHistoryModel()
{
    historyView.ui->lvPomodoroHistory->setModel(historyView.viewModel);
    historyView.ui->lvPomodoroHistory->expandAll();
    historyView.ui->lvPomodoroHistory->show();
}

HistoryStateTask::HistoryStateTask(HistoryView& historyView)
    : HistoryState{historyView}
{
}

void HistoryStateTask::retrieveHistory()
{
    historyView.todoItemModel->setCompletedInIntervalFilter(
        historyView.selectedDateInterval);
    historyView.todoItemModel->select();
    onHistoryRetrieved(historyView.todoItemModel->itemsWithTimestamp());
}

void HistoryStateTask::onHistoryRetrieved(
    const std::vector<std::pair<QDate, QString>>& history)
{
    historyView.fillHistoryModel(history);
    setHistoryModel();
}

void HistoryStateTask::setHistoryModel()
{
    historyView.ui->lvTodoHistory->setModel(historyView.viewModel);
    historyView.ui->lvTodoHistory->expandAll();
    historyView.ui->lvTodoHistory->show();
}
