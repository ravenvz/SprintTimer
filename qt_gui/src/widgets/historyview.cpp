#include "historyview.h"
#include "ui_history.h"
#include <QListView>
#include <QPainter>


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

HistoryView::HistoryView(IPomodoroService& pomodoroService, QWidget* parent)
    : DataWidget(parent)
    , ui(new Ui::HistoryView)
    , pomodoroService{pomodoroService}
    , historyStatePomodoro{std::make_unique<HistoryStatePomodoro>(*this)}
    , historyStateTask{std::make_unique<HistoryStateTask>(*this)}
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    pomodoroService.pomodoroYearRange(std::bind(
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
    historyView.pomodoroService.pomodorosInTimeRange(
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
    const std::vector<Pomodoro>& pomodoros)
{
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
    historyView.setHistoryModel(historyView.ui->lvPomodoroHistory);
}

void HistoryStateTask::onHistoryRetrieved(const std::vector<TodoItem>& tasks)
{
    std::vector<HistoryView::HistoryItem> taskHistory;
    taskHistory.reserve(tasks.size());
    std::transform(
        tasks.cbegin(),
        tasks.cend(),
        std::back_inserter(taskHistory),
        [](const auto& task) {
            return std::make_pair(
                QDateTime::fromTime_t(
                    static_cast<unsigned>(task.lastModified().toTime_t()),
                    Qt::OffsetFromUTC)
                    .date(),
                QString::fromStdString(task.toString()));
        });

    historyView.fillHistoryModel(taskHistory);
    historyView.setHistoryModel(historyView.ui->lvTodoHistory);
}
