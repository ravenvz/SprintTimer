#include "addtodoitemdialog.h"
#include "ui_add_todo_dialog.h"
#include "db_layer/db_service.h"
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
            SLOT(quickAddTag(const QString&)));
    connect(ui->todoName,
            SIGNAL(textEdited(const QString&)),
            this,
            SLOT(resetNameLineEditStyle()));
}

AddTodoItemDialog::~AddTodoItemDialog() { delete ui; }

TodoItem AddTodoItemDialog::getNewTodoItem() { return item; }

void AddTodoItemDialog::accept()
{
    std::string name = ui->todoName->text().toStdString();
    int estimatedPomodoros = ui->estimatedPomodoros->value();
    QString tagsString = ui->leTags->text();

    if (name.empty()) {
        ui->todoName->setStyleSheet("QLineEdit {"
                                    "   border: 2px solid red;"
                                    "}");
    }
    else {
        QStringList qTags = parseTags(tagsString);
        std::list<std::string> tags;
        std::transform(qTags.cbegin(),
                       qTags.cend(),
                       std::back_inserter(tags),
                       [](const auto& tag) { return tag.toStdString(); });
        item = TodoItem{name, estimatedPomodoros, 0, tags, false};
        QDialog::accept();
    }
}

QStringList AddTodoItemDialog::parseTags(QString& tagsString)
{
    // TODO change to return std::list<std::string>
    tagsString.replace(QRegularExpression("(\\s|,)+"), " ");
    QStringList tags;
    if (tagsString != " " && tagsString != "") {
        tags = tagsString.trimmed().split(" ");
    }
    tags.removeDuplicates();
    return tags;
}

void AddTodoItemDialog::fillItemData(TodoItem item)
{
    ui->todoName->setText(QString::fromStdString(item.name()));
    ui->estimatedPomodoros->setValue(item.estimatedPomodoros());
    QStringList qTags;
    std::transform(item.tags().begin(),
                   item.tags().end(),
                   std::back_inserter(qTags),
                   [](const auto& tag) { return QString::fromStdString(tag); });
    ui->leTags->setText(qTags.join(" "));
}

void AddTodoItemDialog::setTagsModel()
{
    ui->tags->setModel(tagModel);
    ui->tags->setModelColumn(1);
    tagModel->select();
    ui->tags->setCurrentText("");
}

void AddTodoItemDialog::quickAddTag(const QString& tag)
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
