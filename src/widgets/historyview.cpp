#include <QPainter>
#include "historyview.h"
#include "ui_history.h"
#include "db_layer/db_service.h"

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

HistoryView::HistoryView(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::HistoryView)
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
    connect(ui->twHistoryDisplay,
            SIGNAL(currentChanged(int)),
            this,
            SLOT(displayHistory()));
    connect(ui->widgetPickPeriod,
            SIGNAL(intervalChanged(DateInterval)),
            this,
            SLOT(onDatePickerIntervalChanged(DateInterval)));
    displayHistory();
}

HistoryView::~HistoryView() { delete ui; }

void HistoryView::updateView() { displayHistory(); }

void HistoryView::displayHistory()
{
    if (ui->twHistoryDisplay->currentIndex() == 0) {
        QVector<HistoryItem> pomodoroHistory = getPomodoroHistory();
        fillHistoryModel(pomodoroHistory);
        setHistoryModel(HistoryType::Pomodoro);
    }
    else {
        QVector<HistoryItem> taskHistory = getTaskHistory();
        fillHistoryModel(taskHistory);
        setHistoryModel(HistoryType::Task);
    }
}

QVector<HistoryView::HistoryItem> HistoryView::getPomodoroHistory() const
{
    pomodoroModel->setDateFilter(selectedDateInterval);
    pomodoroModel->select();
    QVector<Pomodoro> pomodoros{pomodoroModel->items()};
    QVector<HistoryItem> pomodoroHistory;
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
                pomo.toString());
        });
    return pomodoroHistory;
}

QVector<HistoryView::HistoryItem> HistoryView::getTaskHistory() const
{
    todoItemModel->setCompletedInIntervalFilter(selectedDateInterval);
    todoItemModel->select();
    return todoItemModel->itemsWithTimestamp();
}

void HistoryView::fillHistoryModel(const QVector<HistoryItem>& history)
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

void HistoryView::setHistoryModel(const HistoryType& type)
{
    if (type == HistoryType::Pomodoro) {
        ui->lvPomodoroHistory->setModel(viewModel);
        ui->lvPomodoroHistory->expandAll();
        ui->lvPomodoroHistory->show();
    }
    else {
        ui->lvTodoHistory->setModel(viewModel);
        ui->lvTodoHistory->expandAll();
        ui->lvTodoHistory->show();
    }
}

void HistoryView::onDatePickerIntervalChanged(DateInterval newInterval)
{
    selectedDateInterval = newInterval;
    displayHistory();
}
