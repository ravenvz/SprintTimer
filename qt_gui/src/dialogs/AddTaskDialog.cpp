/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
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
    const int estimatedCost = ui->estimatedCost->value();
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

    return Task{name, estimatedCost, 0, tags, false};
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
    ui->estimatedCost->setValue(item.estimatedCost());
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
