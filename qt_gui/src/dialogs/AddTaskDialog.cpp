#include "AddTaskDialog.h"
#include "core/StringUtils.h"
#include "ui_add_todo_dialog.h"
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
    const std::string name = ui->todoName->text().toStdString();
    const int estimatedPomodoros = ui->estimatedPomodoros->value();
    std::list<Tag> tags;
    std::list<std::string> tagNames
        = StringUtils::parseWords(ui->leTags->text().toStdString());
    // Remove duplicate tags if any.
    tagNames.sort();
    tagNames.unique();

    std::transform(tagNames.cbegin(),
                   tagNames.cend(),
                   std::back_inserter(tags),
                   [](const auto& name) { return Tag{name}; });

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
    const auto tags = item.tags();
    std::list<std::string> tagNames;
    std::transform(tags.cbegin(),
                   tags.cend(),
                   std::back_inserter(tagNames),
                   [](const auto& tag) { return tag.name(); });

    QString joined_tags = QString::fromStdString(
        StringUtils::join(tagNames.cbegin(), tagNames.cend(), " "));
    ui->todoName->setText(QString::fromStdString(item.name()));
    ui->estimatedPomodoros->setValue(item.estimatedPomodoros());
    ui->leTags->setText(joined_tags);
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
    // ui->tags->setModelColumn(1);
    // tagModel->select();
    ui->tags->setCurrentText("");
}
