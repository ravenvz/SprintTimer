#include "addtodoitemdialog.h"
#include "ui_add_todo_dialog.h"
#include "db_layer/db_helper.h"
#include <QRegularExpression>

AddTodoItemDialog::AddTodoItemDialog(TagModel* tagModel, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddTodoItemDialog),
    tagModel(tagModel)
{
    ui->setupUi(this);
    setTagsModel();
    connect(ui->tags, SIGNAL(activated(const QString&)), this, SLOT(quickAddTag(const QString&)));
    connect(ui->todoName, SIGNAL(textEdited(const QString&)), this, SLOT(resetNameLineEditStyle()));
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
    int estimatedPomodoros = ui->estimatedPomodoros->value();
    QString tagsString = ui->leTags->text();

    if (name.isEmpty()) {
        ui->todoName->setStyleSheet("QLineEdit {"
                                    "   border: 2px solid red;"
                                    "}");
    } else {
        QStringList tags = parseTags(tagsString);
        item = TodoItem {name, estimatedPomodoros, 0, tags, false};
        QDialog::accept();
    }
}

QStringList AddTodoItemDialog::parseTags(QString& tagsString) {
    tagsString.replace(QRegularExpression("(\\s|,)+"), " ");
    QStringList tags;
    if (tagsString != " " && tagsString != "") {
        tags = tagsString.trimmed().split(" ");
    }
    tags.removeDuplicates();
    return tags;
}

void AddTodoItemDialog::fillItemData(TodoItem item) {
    ui->todoName->setText(item.name());
    ui->estimatedPomodoros->setValue(item.estimatedPomodoros());
    ui->leTags->setText(item.tags().join(" "));
}

void AddTodoItemDialog::setTagsModel() {
    ui->tags->setModel(tagModel);
    ui->tags->setModelColumn(1);
    tagModel->select();
    ui->tags->setCurrentText("");
}

void AddTodoItemDialog::quickAddTag(const QString& tag) {
    QString prevTag = ui->leTags->text();
    if (!prevTag.isEmpty()) {
        prevTag.append(" ");
    }
    prevTag.append(tag);
    ui->leTags->setText(prevTag);
}

void AddTodoItemDialog::resetNameLineEditStyle() {
    ui->todoName->setStyleSheet("");
}
