#include <QtCore/qdatetime.h>
#include <QtCore/qstringlistmodel.h>
#include "pickperiodwidget.h"
#include "ui_pickperiodwidget.h"
#include <algorithm>
#include <iterator>
#include <memory>
//#include "TaskScheduler.h"


PickPeriodWidget::PickPeriodWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::PickPeriodWidget),
    selectedInterval(DateInterval {QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1),
                                   QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1).addMonths(1).addDays(-1)})
{
    ui->setupUi(this);
    // TODO replace with Qt-generated months names
    QStringList months;
    for (int monthNumber = 1; monthNumber <= 12; ++monthNumber) {
        months.append(QDate::longMonthName(monthNumber));
    }
    monthsModel = new QStringListModel(months);
    ui->cbxMonth->setModel(monthsModel);
    connectSlots();
}

PickPeriodWidget::~PickPeriodWidget()
{
    delete yearsModel;
    delete monthsModel;
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

DateInterval PickPeriodWidget::getInterval() const {
    return selectedInterval;
}

void PickPeriodWidget::setYears(const QStringList& years) {
    if (yearsModel) {
        delete yearsModel;
    }
    yearsModel = new QStringListModel(years);
    ui->cbxYear->setModel(yearsModel);
    ui->cbxYear->setCurrentIndex(std::distance(years.begin(),
                std::find(years.begin(), years.end(), QString("%1").arg(QDate::currentDate().year())))); 
    ui->cbxMonth->setCurrentIndex(QDate::currentDate().month() - 1);
    updateInterval();
}
