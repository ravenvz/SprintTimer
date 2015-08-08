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

private slots:
    void showCalendar();

private:
    Ui::DatePickDialog *ui;

    void configureCalendar();

};

#endif // DATEPICKDIALOG_H
