/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "qt_gui/dialogs/AddTaskDialog.h"
#include "ui_add_todo_dialog.h"
#include <QRegularExpression>
#include <core/utils/StringUtils.h>

namespace {

const QString requiredFieldEmptyStyle{"QLineEdit { border: 2px solid red; }"};

} // namespace

namespace sprint_timer::ui::qt_gui {

using namespace entities;
using namespace utils;

AddTaskDialog::AddTaskDialog(QAbstractItemModel& tagModel, QWidget* parent)
    : QDialog{parent}
    , ui{std::make_unique<Ui::AddTaskDialog>()}
{
    ui->setupUi(this);

    ui->tags->setModel(&tagModel);
    ui->tags->setCurrentText("");

    connect(ui->tags,
            QOverload<const QString&>::of(&QComboBox::activated),
            this,
            &AddTaskDialog::onQuickAddTagActivated);
    connect(ui->taskName,
            &QLineEdit::textEdited,
            this,
            &AddTaskDialog::resetNameLineEditStyle);
}

AddTaskDialog::~AddTaskDialog() = default;

Task AddTaskDialog::constructedTask()
{
    const std::string name = ui->taskName->text().toStdString();
    const int estimatedCost = ui->estimatedCost->value();
    std::list<Tag> tags;
    std::list<std::string> tagNames
        = parseWords(ui->leTags->text().toStdString());
    // Remove duplicate tags if any.
    tagNames.sort();
    tagNames.unique();

    std::transform(tagNames.cbegin(),
                   tagNames.cend(),
                   std::back_inserter(tags),
                   [](const auto& name) { return Tag{name}; });

    return Task{name, estimatedCost, 0, std::move(tags), false};
}

void AddTaskDialog::accept()
{
    QString name = ui->taskName->text();
    if (name.isEmpty())
        ui->taskName->setStyleSheet(requiredFieldEmptyStyle);
    else {
        QDialog::accept();
    }
    resetDataFields();
}

void AddTaskDialog::fillItemData(const Task& item)
{
    const auto tags = item.tags();
    std::list<std::string> tagNames;
    std::transform(tags.cbegin(),
                   tags.cend(),
                   std::back_inserter(tagNames),
                   [](const auto& tag) { return tag.name(); });

    QString joined_tags
        = QString::fromStdString(join(tagNames.cbegin(), tagNames.cend(), " "));
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

void AddTaskDialog::resetDataFields()
{
    ui->taskName->clear();
    ui->leTags->clear();
    ui->tags->setCurrentIndex(-1);
    ui->estimatedCost->setValue(1);
}

} // namespace sprint_timer::ui::qt_gui
