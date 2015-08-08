#include "historyview.h"
#include "ui_history.h"
#include <QStringList>
#include <QDebug>
#include "data_base/db_helper.h"


HistoryView::HistoryView(QDialog* parent) : 
    QDialog(parent),
    ui(new Ui::HistoryView)
{
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
}

HistoryView::~HistoryView() {
    delete yearsModel;
    delete monthsModel;
    delete pomodoroModel;
    delete ui;
}

void HistoryView::displayHistory() {
    ui->twHistoryDisplay->currentIndex() == 0 ? populatePomodoroHistory() : populateTodoHistory();
}

void HistoryView::populatePomodoroHistory() {
    QString year = ui->cbxYear->currentText();
    QString month = QString("%1").arg(ui->cbxMonth->currentIndex() + 1, 2, 10, QChar('0'));
    QStringList preprocessedHistory;
    getPomodoroHistory(year, month, preprocessedHistory);
    pomodoroModel = new QStringListModel(preprocessedHistory);
    ui->lvPomodoroHistory->setModel(pomodoroModel);
    ui->lvPomodoroHistory->show();
}

void HistoryView::getPomodoroHistory(const QString& year,
                                     const QString& month,
                                     QStringList& preprocessedHistory) const {
    QVector<Pomodoro> pomodorosForPeriod = PomodoroGateway::getPomodoroForMonth(year, month);
    if (!pomodorosForPeriod.isEmpty()) {
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

}
