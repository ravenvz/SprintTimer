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
    // this->setWindowModality(Qt::ApplicationModal);
    ui->setupUi(this);
    QStringList years {"2015", "2016"};
    yearsModel = new QStringListModel(years);
    ui->cbxYear->setModel(yearsModel);
    ui->cbxYear->setCurrentIndex(0);
    QStringList months {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    monthsModel = new QStringListModel(months);
    ui->cbxMonth->setModel(monthsModel);
    ui->cbxMonth->setCurrentIndex(0);
    ui->lvTodoHistory->hide();
    ui->lvPomodoroHistory->hide();
    connect(ui->btnDisplayHistory, SIGNAL(clicked(bool)), this, SLOT(displayHistory()));
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
    QString year = getPeriodMonth();
    QString month = getPeriodYear();
    QStringList preprocessedHistory;
    getPomodoroHistory(year, month, preprocessedHistory);
    QPointer<QStringListModel> pomodoroModel = new QStringListModel(preprocessedHistory);
    ui->lvPomodoroHistory->setModel(pomodoroModel);
    ui->lvPomodoroHistory->show();
}

QString HistoryView::getPeriodYear() const {
    QString month = QString("%1").arg(ui->cbxMonth->currentIndex() + 1, 2, 10, QChar('0'));
    return month;
}

QString HistoryView::getPeriodMonth() const {
    QString year = ui->cbxYear->currentText();
    return year;
}

void HistoryView::getPomodoroHistory(const QString& year,
                                     const QString& month,
                                     QStringList& preprocessedHistory) const {
    QVector<Pomodoro> pomodorosForPeriod = PomodoroGateway::getPomodoroForMonth(year, month);
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
    QVector<unsigned> headerIndexes {0};
    for (int i = 0; i < pomodoros.size(); ++i) {
        if (currentDate != pomodoros[i].startTime.date()) {
            currentDate = pomodoros[i].startTime.date();
            preparedPomodoroHistory << pomodoros[i].startTime.date().toString();
            headerIndexes << i;
        }
        preparedPomodoroHistory << pomodoros[i].asString();
    }
}

void HistoryView::populateTodoHistory() {
    QString year = getPeriodMonth();
    QString month = getPeriodYear();
    QStringList preprocessedHistory;
    getTodoItemsHistory(year, month, preprocessedHistory);
    QPointer<QStringListModel> todoItemModel = new QStringListModel(preprocessedHistory);
    ui->lvTodoHistory->setModel(todoItemModel);
    ui->lvTodoHistory->show();
}

void HistoryView::openDatePicker() {
    DatePickDialog dialog;
    dialog.exec();
}

void HistoryView::getTodoItemsHistory(const QString& year, const QString& month, QStringList& formattedHistory) {
    QVector<std::pair<TodoItem, QString> > todoItemsForPeriod = TodoItemGateway::getTodoItemsForMonth(year, month);
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
    QVector<unsigned> headerIndexes {0};
    for (int i = 0; i < todoItemsForPeriod.size(); ++i) {
        if (currentDate != todoItemsForPeriod[i].second) {
            currentDate = todoItemsForPeriod[i].second;
            formattedHistory << currentDate;
            headerIndexes << i;
        }
        formattedHistory << todoItemsForPeriod[i].first.asString();
    }

}
