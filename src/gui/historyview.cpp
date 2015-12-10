#include "historyview.h"
#include "ui_history.h"
#include "db_layer/db_helper.h"


HistoryView::HistoryView(QWidget* parent) : 
    QWidget(parent),
    ui(new Ui::HistoryView)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    selectedDateInterval = ui->widgetPickPeriod->getInterval();
    displayHistory();
    connectSlots();
}

void HistoryView::connectSlots() {
    connect(ui->twHistoryDisplay, SIGNAL(currentChanged(int)), this, SLOT(displayHistory()));
    connect(ui->widgetPickPeriod, SIGNAL(intervalChanged(DateInterval)), this, SLOT(
            onDatePickerIntervalChanged(DateInterval)));
}

HistoryView::~HistoryView() {
    delete ui;
}

void HistoryView::updateView() {
    displayHistory();
}

void HistoryView::displayHistory() {
    ui->twHistoryDisplay->currentIndex() == 0 ? populatePomodoroHistory() : populateTodoHistory();
}

void HistoryView::populatePomodoroHistory() {
    QStringList preprocessedHistory;
    getPomodoroHistory(preprocessedHistory);
    QPointer<QStringListModel> pomodoroModel = new QStringListModel(preprocessedHistory);
    ui->lvPomodoroHistory->setModel(pomodoroModel);
    ui->lvPomodoroHistory->show();
}

void HistoryView::getPomodoroHistory(QStringList& preprocessedHistory) const {
    QVector<Pomodoro> pomodorosForInterval = PomodoroDataSource::getPomodorosBetween(
            selectedDateInterval.startDate,
            selectedDateInterval.endDate);
    if (!pomodorosForInterval.isEmpty()) {
        preprocessedHistory << QString("Completed %1 pomodoros").arg(pomodorosForInterval.size());
        formatPomodoroHistory(pomodorosForInterval, preprocessedHistory);
    } else {
        preprocessedHistory << "No data stored for selected period";
    }
}

void HistoryView::formatPomodoroHistory(const QVector<Pomodoro> &pomodoros, QStringList &preparedPomodoroHistory) const {
    preparedPomodoroHistory << pomodoros[0].getStartTime().date().toString();
    QDate currentDate = pomodoros[0].getStartTime().date();
    unsigned headerOffset = 1;
    QVector<unsigned> headerIndexes {headerOffset};
    for (int i = 0; i < pomodoros.size(); ++i) {
        const QDate& date = pomodoros[i].getStartTime().date();
        if (currentDate != date) {
            currentDate = date;
            preparedPomodoroHistory << date.toString();
            headerIndexes << i + headerOffset;
        }
        preparedPomodoroHistory << pomodoros[i].asString();
    }
}

void HistoryView::populateTodoHistory() {
    QStringList preprocessedHistory;
    getTodoItemsHistory(preprocessedHistory);
    QPointer<QStringListModel> todoItemModel = new QStringListModel(preprocessedHistory);
    ui->lvTodoHistory->setModel(todoItemModel);
    ui->lvTodoHistory->show();
}

void HistoryView::getTodoItemsHistory(QStringList& formattedHistory) {
    QVector<std::pair<TodoItem, QString> > todoItemsForPeriod = TodoItemDataSource::getTodoItemsForMonth(
            selectedDateInterval.startDate,
            selectedDateInterval.endDate);
    if (!todoItemsForPeriod.isEmpty()) {
        formattedHistory << QString("Completed %1 items").arg(todoItemsForPeriod.size());
        formatTodoItemHistory(todoItemsForPeriod, formattedHistory);
    } else {
        formattedHistory << "No data stored for selected period";
    }
}

void HistoryView::formatTodoItemHistory(const QVector<std::pair<TodoItem, QString> > todoItemsForPeriod, QStringList& formattedHistory) {
    QString currentDate = todoItemsForPeriod[0].second;
    formattedHistory << currentDate;
    unsigned headerOffset = 1;
    QVector<unsigned> headerIndexes {headerOffset};
    for (int i = 0; i < todoItemsForPeriod.size(); ++i) {
        if (currentDate != todoItemsForPeriod[i].second) {
            currentDate = todoItemsForPeriod[i].second;
            formattedHistory << currentDate;
            headerIndexes << i + headerOffset;
        }
        formattedHistory << todoItemsForPeriod[i].first.asString();
    }
}

void HistoryView::onDatePickerIntervalChanged(DateInterval newInterval) {
    selectedDateInterval = newInterval;
    displayHistory();
}
