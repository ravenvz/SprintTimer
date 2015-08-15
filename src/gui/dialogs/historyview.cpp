#include "historyview.h"
#include "ui_history.h"
#include <QStringList>
#include <QDebug>
#include "data_base/db_helper.h"
#include "gui/dialogs/datepickdialog.h"


HistoryView::HistoryView(QWidget* parent) : 
    QWidget(parent),
    ui(new Ui::HistoryView)
{
    ui->setupUi(this);
    // TODO years should be computed dynamically
    QStringList years {"2015", "2016"};
    yearsModel = new QStringListModel(years);
    ui->cbxYear->setModel(yearsModel);
    ui->cbxYear->setCurrentIndex(0);
    QStringList months {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    monthsModel = new QStringListModel(months);
    ui->cbxMonth->setModel(monthsModel);
    ui->cbxMonth->setCurrentIndex(QDate::currentDate().month() - 1);
    updatePeriod();
    displayHistory();
    connect(ui->cbxYear, SIGNAL(activated(int)), this, SLOT(updatePeriod()));
    connect(ui->cbxMonth, SIGNAL(activated(int)), this, SLOT(updatePeriod()));
    connect(ui->cbxYear, SIGNAL(activated(int)), this, SLOT(displayHistory()));
    connect(ui->cbxMonth, SIGNAL(activated(int)), this, SLOT(displayHistory()));
    connect(ui->twHistoryDisplay, SIGNAL(currentChanged(int)), this, SLOT(displayHistory()));
    connect(ui->btnPickPeriod, SIGNAL(clicked(bool)), this, SLOT(openDatePicker()));
}

HistoryView::~HistoryView() {
    delete yearsModel;
    delete monthsModel;
    delete ui;
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
    QVector<Pomodoro> pomodorosForPeriod = PomodoroGateway::getPomodoroForMonth(startDate, endDate);
    if (!pomodorosForPeriod.isEmpty()) {
        preprocessedHistory << QString("Completed %1 pomodoros").arg(pomodorosForPeriod.size());
        formatPomodoroHistory(pomodorosForPeriod, preprocessedHistory);
    } else {
        preprocessedHistory << "No data stored for selected period";
    }
}

void HistoryView::formatPomodoroHistory(const QVector<Pomodoro> &pomodoros, QStringList &preparedPomodoroHistory) const {
    preparedPomodoroHistory << pomodoros[0].startTime.date().toString();
    QDate currentDate = pomodoros[0].startTime.date();
    unsigned headerOffset = 1;
    QVector<unsigned> headerIndexes {headerOffset};
    for (int i = 0; i < pomodoros.size(); ++i) {
        if (currentDate != pomodoros[i].startTime.date()) {
            currentDate = pomodoros[i].startTime.date();
            preparedPomodoroHistory << pomodoros[i].startTime.date().toString();
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
    QVector<std::pair<TodoItem, QString> > todoItemsForPeriod = TodoItemGateway::getTodoItemsForMonth(startDate, endDate);
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

void HistoryView::updatePeriod(const QDate& start, const QDate& end) {
    startDate = start;
    endDate = end;
}

void HistoryView::updatePeriod() {
    startDate = QDate(ui->cbxYear->currentText().toInt(), ui->cbxMonth->currentIndex() + 1, 1);
    endDate = startDate.addDays(startDate.daysInMonth());
}

void HistoryView::openDatePicker() {
    DatePickDialog dialog;
    if (dialog.exec()) {
        std::pair<QDate, QDate> period = dialog.getNewPeriod();
        updatePeriod(period.first, period.second);
        displayHistory();
    }
}

