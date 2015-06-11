#ifndef ADDTODOITEMDIALOG_H
#define ADDTODOITEMDIALOG_H

#include <QDialog>
#include "core/entities.h"

namespace Ui {
class AddTodoItemDialog;
}

class AddTodoItemDialog : public QDialog
{
    Q_OBJECT  // remove if no signal/slots here

public:
    explicit AddTodoItemDialog(QWidget* parent = 0);
    ~AddTodoItemDialog();
    void accept() override;
    TodoItem getNewTodoItem();
    void fillItemData(TodoItem item);

private:
    Ui::AddTodoItemDialog *ui;
    TodoItem item;
};

#endif // ADDTODOITEMDIALOG_H
