#include "AddTaskDialog.h"
#include "core/StringUtils.h"
#include "ui_add_todo_dialog.h"
#include <QRegularExpression>


AddTaskDialog::AddTaskDialog(TagModel* tagModel, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AddTaskDialog)
    , tagModel(tagModel)
{
    ui->setupUi(this);
    setTagsModel();
    connect(ui->tags,
            SIGNAL(activated(const QString&)),
            this,
            SLOT(onQuickAddTagActivated(const QString&)));
    connect(ui->taskName,
            SIGNAL(textEdited(const QString&)),
            this,
            SLOT(resetNameLineEditStyle()));
}

AddTaskDialog::~AddTaskDialog() { delete ui; }

Task AddTaskDialog::constructedTask()
{
    const std::string name = ui->taskName->text().toStdString();
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

    return Task{name, estimatedPomodoros, 0, tags, false};
}

void AddTaskDialog::accept()
{
    QString name = ui->taskName->text();
    name.isEmpty() ? ui->taskName->setStyleSheet(requiredFieldEmptyStyle)
                   : QDialog::accept();
}

void AddTaskDialog::fillItemData(const Task& item)
{
    const auto tags = item.tags();
    std::list<std::string> tagNames;
    std::transform(tags.cbegin(),
                   tags.cend(),
                   std::back_inserter(tagNames),
                   [](const auto& tag) { return tag.name(); });

    QString joined_tags = QString::fromStdString(
        StringUtils::join(tagNames.cbegin(), tagNames.cend(), " "));
    ui->taskName->setText(QString::fromStdString(item.name()));
    ui->estimatedPomodoros->setValue(item.estimatedPomodoros());
    ui->leTags->setText(joined_tags);
}

void AddTaskDialog::onQuickAddTagActivated(const QString& tag)
{
    QString prevTag = ui->leTags->text();
    if (!prevTag.isEmpty()) {
        prevTag.append(" ");
    }
    prevTag.append(tag);
    ui->leTags->setText(prevTag);
}

void AddTaskDialog::resetNameLineEditStyle()
{
    ui->taskName->setStyleSheet("");
}

void AddTaskDialog::setTagsModel()
{
    ui->tags->setModel(tagModel);
    // ui->tags->setModelColumn(1);
    // tagModel->select();
    ui->tags->setCurrentText("");
}
