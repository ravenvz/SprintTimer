#include "datepickdialog.h"
#include "ui_date_pick_dialog.h"

DatePickDialog::DatePickDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DatePickDialog)
{
    ui->setupUi(this);
    ui->calendarWidget->hide();
    connect(ui->btnPickStartDate, SIGNAL(clicked(bool)), this, SLOT(showCalendar()));
    connect(ui->btnPickEndDate, SIGNAL(clicked(bool)), this, SLOT(showCalendar()));
}

DatePickDialog::~DatePickDialog()
{
    delete ui;
}

void DatePickDialog::configureCalendar() {
    // set max date
    // set first day of week (possibly with global settings)

}

void DatePickDialog::showCalendar() {
    ui->calendarWidget->show();
}
