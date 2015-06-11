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

TodoItem AddTodoItemDialog::getNewTodoItem() {
    return item;
}

void AddTodoItemDialog::accept() {
    QString name = ui->todoName->text();
    unsigned estimatedPomodoros = ui->estimatedPomodoros->value();
    QString tagsString = ui->tags->currentText();

    if (!name.isEmpty()) {
        QStringList tags = tagsString.trimmed().split(" ");
        item = TodoItem {name, estimatedPomodoros, 0, 0, tags, false, AddTodoItemDialog::tempId};
        QDialog::accept();
    }
}

void AddTodoItemDialog::fillItemData(TodoItem item) {
    ui->todoName->setText(item.name);
    ui->estimatedPomodoros->setValue(item.estimatedPomodoros);
    ui->tags->setCurrentText(item.tagsAsString());
}
