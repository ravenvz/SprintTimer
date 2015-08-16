#include <QtCore/qdatetime.h>
#include "datepickdialog.h"
#include "ui_date_pick_dialog.h"

DatePickDialog::DatePickDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DatePickDialog)
{
    ui->setupUi(this);
    ui->dePickEndDate->setDate(QDate::currentDate());
    ui->dePickStartDate->setDate(QDate::currentDate().addMonths(-1));
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

std::pair<QDate, QDate> DatePickDialog::getNewPeriod() {
    return std::make_pair(ui->cwStart->selectedDate(), ui->cwEnd->selectedDate());
}
