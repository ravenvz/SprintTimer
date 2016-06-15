#ifndef DATEPICKDIALOG_H
#define DATEPICKDIALOG_H

#include "utils/DateInterval.h"
#include <QDialog>

namespace Ui {
class DatePickDialog;
}


class DatePickDialog : public QDialog {
    Q_OBJECT

public:
    explicit DatePickDialog(DateInterval initialPeriod, QWidget* parent = nullptr);
    ~DatePickDialog();
    DateInterval getNewInterval();

private:
    Ui::DatePickDialog* ui;

    void configureCalendar();
    void updateCalendarDates(DateInterval& period);
};

#endif // DATEPICKDIALOG_H
