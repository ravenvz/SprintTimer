#ifndef DATEPICKDIALOG_H
#define DATEPICKDIALOG_H

#include <QDialog>

namespace Ui {
class DatePickDialog;
}

class DatePickDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatePickDialog(QWidget* parent = 0);
    ~DatePickDialog();
    std::pair<QDate, QDate> getNewPeriod();

private slots:

private:
    Ui::DatePickDialog *ui;

    void configureCalendar();

};

#endif // DATEPICKDIALOG_H
