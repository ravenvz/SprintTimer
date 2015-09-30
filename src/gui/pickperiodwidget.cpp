#include <QtCore/qdatetime.h>
#include <QtCore/qstringlistmodel.h>
#include "pickperiodwidget.h"
#include "ui_pickperiodwidget.h"
//#include "TaskScheduler.h"


PickPeriodWidget::PickPeriodWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::PickPeriodWidget),
    selectedInterval(DateInterval {QDate::currentDate().addDays(-30), QDate::currentDate()})
{
    ui->setupUi(this);
    QStringList years {"2015", "2016"};
    QStringListModel* yearsModel = new QStringListModel(years);
    ui->cbxYear->setModel(yearsModel);
    ui->cbxYear->setCurrentIndex(0);
    // TODO replace with Qt-generated months names
    QStringList months {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    QStringListModel* monthsModel = new QStringListModel(months);
    ui->cbxMonth->setModel(monthsModel);
    ui->cbxMonth->setCurrentIndex(QDate::currentDate().month() - 1);
    updateInterval();
    connectSlots();
}

PickPeriodWidget::~PickPeriodWidget()
{
    delete ui;
}

void PickPeriodWidget::connectSlots() {
    connect(ui->btnPickPeriod, SIGNAL(clicked(bool)), this, SLOT(openDatePickDialog()));
    connect(ui->cbxYear, SIGNAL(activated(int)), this, SLOT(updateInterval()));
    connect(ui->cbxMonth, SIGNAL(activated(int)), this, SLOT(updateInterval()));
    connect(this, SIGNAL(intervalChanged(DateInterval)), this, SLOT(updateSelectionHintLabel()));
}

void PickPeriodWidget::openDatePickDialog() {
    DatePickDialog dialog {selectedInterval};
    if (dialog.exec()) {
        updateInterval(dialog.getNewInterval());
    }
}

void PickPeriodWidget::updateSelectionHintLabel() {
    ui->labelSelectionHint->setText(selectedInterval.toString());
}

void PickPeriodWidget::setInterval(DateInterval interval) {
    selectedInterval = interval;
    emit intervalChanged(selectedInterval);
}

void PickPeriodWidget::updateInterval() {
    QDate startDate {ui->cbxYear->currentText().toInt(), ui->cbxMonth->currentIndex() + 1, 1};
    QDate endDate = startDate.addDays(startDate.daysInMonth() - 1);
    selectedInterval = DateInterval {startDate, endDate};
    emit intervalChanged(selectedInterval);
}

void PickPeriodWidget::updateInterval(DateInterval interval) {
    selectedInterval = interval;
    emit intervalChanged(selectedInterval);
}
