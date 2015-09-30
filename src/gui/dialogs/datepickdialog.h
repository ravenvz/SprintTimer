#ifndef DATEPICKDIALOG_H
#define DATEPICKDIALOG_H

#include <QDialog>
#include "core/TaskScheduler.h"

namespace Ui {
    class DatePickDialog;
}


class DatePickDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatePickDialog(DateInterval initialPeriod, QWidget* parent = 0);
    ~DatePickDialog();
    DateInterval getNewInterval();

private:
    Ui::DatePickDialog *ui;

    void configureCalendar();
    void updateCalendarDates(DateInterval& period);

};

#endif // DATEPICKDIALOG_H
