#include <QtCore/qdatetime.h>
#include "datepickdialog.h"
#include "ui_date_pick_dialog.h"

DatePickDialog::DatePickDialog(DateInterval initialPeriod, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DatePickDialog)
{
    ui->setupUi(this);
    updateCalendarDates(initialPeriod);
    configureCalendar();
}

DatePickDialog::~DatePickDialog()
{
    delete ui;
}

void DatePickDialog::configureCalendar() {
    ui->cwStart->setMaximumDate(QDate::currentDate());
    ui->cwEnd->setMaximumDate(QDate::currentDate());

    // set first day of week TODO (make this configurable with global settings)
    ui->cwStart->setFirstDayOfWeek(Qt::Monday);
    ui->cwEnd->setFirstDayOfWeek(Qt::Monday);
}

void DatePickDialog::updateCalendarDates(DateInterval& period) {
    ui->dePickStartDate->setDate(period.startDate);
    ui->dePickEndDate->setDate(period.endDate);
}

DateInterval DatePickDialog::getNewInterval() {
    return DateInterval {ui->cwStart->selectedDate(), ui->cwEnd->selectedDate()};
}
