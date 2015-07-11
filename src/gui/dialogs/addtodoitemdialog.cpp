#include "addtodoitemdialog.h"
#include "ui_add_todo_dialog.h"
#include <QStringListModel>
#include <data_base/db_helper.h>

AddTodoItemDialog::AddTodoItemDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddTodoItemDialog)
{
    ui->setupUi(this);
    tagModel = new QStringListModel(this);
    setTagsModel();
    connect(ui->tags, SIGNAL(activated(const QString&)), this, SLOT(quickAddTag(const QString&)));
}

AddTodoItemDialog::~AddTodoItemDialog()
{
    delete tagModel;
    delete ui;
}

TodoItem AddTodoItemDialog::getNewTodoItem() {
    return item;
}

void AddTodoItemDialog::accept() {
    QString name = ui->todoName->text();
    unsigned estimatedPomodoros = ui->estimatedPomodoros->value();
    QString tagsString = ui->leTags->text();

    if (!name.isEmpty()) {
        QStringList tags = tagsString.trimmed().split(" ");
        item = TodoItem {name, estimatedPomodoros, 0, tags, false, AddTodoItemDialog::tempId};
        QDialog::accept();
    }
}

void AddTodoItemDialog::fillItemData(TodoItem item) {
    ui->todoName->setText(item.name);
    ui->estimatedPomodoros->setValue(item.estimatedPomodoros);
    ui->leTags->setText(item.tagsAsString());
}

void AddTodoItemDialog::setTagsModel() {
    QStringList lst = TagGateway::getAllTags();
    tagModel->setStringList(lst);
    ui->tags->setModel(tagModel);
    ui->tags->setCurrentText("");
}

void AddTodoItemDialog::quickAddTag(const QString& tag) {
    QString prevTag = ui->leTags->text();
    prevTag.append(" ");
    prevTag.append(tag);
    ui->leTags->setText(prevTag);
}
