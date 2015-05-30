#ifndef POMODOROCANCELDIALOG_H
#define POMODOROCANCELDIALOG_H

#include <QDialog>

namespace Ui {
class PomodoroCancelDialog;
}

class PomodoroCancelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PomodoroCancelDialog(QWidget* parent = 0);
    ~PomodoroCancelDialog();

private:
    Ui::PomodoroCancelDialog *ui;
};

#endif // POMODOROCANCELDIALOG_H
