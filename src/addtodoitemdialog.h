#ifndef ADDTODOITEMDIALOG_H
#define ADDTODOITEMDIALOG_H

#include <QDialog>

namespace Ui {
class AddTodoItemDialog;
}

class AddTodoItemDialog : public QDialog
{
    Q_OBJECT  // remove if no signal/slots here

public:
    explicit AddTodoItemDialog(QWidget* parent = 0);
    ~AddTodoItemDialog();

private:
    Ui::AddTodoItemDialog *ui;
};

#endif // ADDTODOITEMDIALOG_H
