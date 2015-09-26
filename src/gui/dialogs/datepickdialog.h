#ifndef DATEPICKDIALOG_H
#define DATEPICKDIALOG_H

#include <QDialog>

namespace Ui {
    class DatePickDialog;
}


struct DateInterval {
    QDate startDate;
    QDate endDate;

    QString toString() {
        return QString("%1 - %2").arg(startDate.toString()).arg(endDate.toString());
    }
};


class DatePickDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatePickDialog(DateInterval initialPeriod, QWidget* parent = 0);
    ~DatePickDialog();
    DateInterval getNewInterval();

private slots:

private:
    Ui::DatePickDialog *ui;

    void configureCalendar();
    void updateCalendarDates(DateInterval& period);

};

#endif // DATEPICKDIALOG_H
