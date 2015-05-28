#include "addtodoitemdialog.h"
#include "ui_add_todo_dialog.h"

AddTodoItemDialog::AddTodoItemDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddTodoItemDialog)
{
    ui->setupUi(this);
}

AddTodoItemDialog::~AddTodoItemDialog()
{
    delete ui;
}
