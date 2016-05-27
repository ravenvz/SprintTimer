#include "AddTaskDialog.h"
#include "ui_add_todo_dialog.h"
#include "core/StringUtils.h"
#include <QRegularExpression>

AddTodoItemDialog::AddTodoItemDialog(TagModel* tagModel, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AddTodoItemDialog)
    , tagModel(tagModel)
{
    ui->setupUi(this);
    setTagsModel();
    connect(ui->tags,
            SIGNAL(activated(const QString&)),
            this,
            SLOT(onQuickAddTagActivated(const QString&)));
    connect(ui->todoName,
            SIGNAL(textEdited(const QString&)),
            this,
            SLOT(resetNameLineEditStyle()));
}

AddTodoItemDialog::~AddTodoItemDialog() { delete ui; }

TodoItem AddTodoItemDialog::constructedTask()
{
    std::string name = ui->todoName->text().toStdString();
    int estimatedPomodoros = ui->estimatedPomodoros->value();
    std::list<std::string> tags
        = StringUtils::parseWords(ui->leTags->text().toStdString());
    // Remove duplicate tags if any.
    tags.sort();
    tags.unique();
    return TodoItem{name, estimatedPomodoros, 0, tags, false};
}

void AddTodoItemDialog::accept()
{
    QString name = ui->todoName->text();
    name.isEmpty() ? ui->todoName->setStyleSheet(requiredFieldEmptyStyle)
                   : QDialog::accept();
}

void AddTodoItemDialog::fillItemData(const TodoItem& item)
{
    std::cout << item.toString() << std::endl;
    ui->todoName->setText(QString::fromStdString(item.name()));
    ui->estimatedPomodoros->setValue(item.estimatedPomodoros());

    ui->leTags->setText(QString::fromStdString(item.tagsAsString()));
}

void AddTodoItemDialog::onQuickAddTagActivated(const QString& tag)
{
    QString prevTag = ui->leTags->text();
    if (!prevTag.isEmpty()) {
        prevTag.append(" ");
    }
    prevTag.append(tag);
    ui->leTags->setText(prevTag);
}

void AddTodoItemDialog::resetNameLineEditStyle()
{
    ui->todoName->setStyleSheet("");
}

void AddTodoItemDialog::setTagsModel()
{
    ui->tags->setModel(tagModel);
    ui->tags->setModelColumn(1);
    tagModel->select();
    ui->tags->setCurrentText("");
}
