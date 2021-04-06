/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "qt_gui/dialogs/TaskDialog.h"
#include <QAbstractItemModel>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <core/utils/StringUtils.h>

namespace {

const QString requiredFieldEmptyStyle{"QLineEdit { border: 2px solid red; }"};

constexpr int layoutSpacing{25};
constexpr int maxVisibleItems{15};

} // namespace

namespace sprint_timer::ui::qt_gui {

using namespace utils;

TaskDialog::TaskDialog(QAbstractItemModel& tagModel_, QWidget* parent_)
    : DisplayableDialog{parent_}
    , name{std::make_unique<QLineEdit>().release()}
    , cost{std::make_unique<QSpinBox>().release()}
    , tags{std::make_unique<QLineEdit>().release()}
{
    auto layout_ = std::make_unique<QFormLayout>();

    auto tagBox_ = std::make_unique<QComboBox>();
    auto buttons_ = std::make_unique<QDialogButtonBox>(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    tagBox_->setMaxVisibleItems(maxVisibleItems);
    tagBox_->setEditable(true);
    tagBox_->setModel(&tagModel_);
    name->setPlaceholderText("Task name (required)");
    cost->setMinimum(1);

    connect(
        buttons_.get(), &QDialogButtonBox::accepted, this, &QDialog::accept);

    connect(
        buttons_.get(), &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(
        name, &QLineEdit::textEdited, [this]() { name->setStyleSheet(""); });

    connect(
        tagBox_.get(), &QComboBox::textActivated, [this](const QString& tag) {
            QString prevTag = tags->text();
            if (!prevTag.isEmpty()) {
                prevTag.append(" ");
            }
            prevTag.append(tag);
            tags->setText(prevTag);
        });

    layout_->addRow(name);
    layout_->addRow("Estimated cost", cost);
    layout_->addRow("Tags", tags);
    layout_->addRow("Quick add tag", tagBox_.release());
    layout_->addRow(buttons_.release());
    layout_->setSpacing(layoutSpacing);

    setLayout(layout_.release());
}

TaskDTO TaskDialog::parseFormFields() const
{
    TaskDTO task;
    task.name = name->text().toStdString();
    task.expectedCost = cost->value();
    task.tags.clear();
    const auto tagStr = tags->text().toStdString();
    parseWords(cbegin(tagStr), cend(tagStr), std::back_inserter(task.tags));
    return task;
}

void TaskDialog::fillFormFields(const TaskDTO& task)
{
    name->setText(QString::fromStdString(task.name));
    cost->setValue(task.expectedCost);
    QString joined_tags =
        QString::fromStdString(join(task.tags.cbegin(), task.tags.cend(), " "));
    tags->setText(joined_tags);
}

bool TaskDialog::nameIsEmpty() const { return name->text().isEmpty(); }

void TaskDialog::markNameFieldRed()
{
    name->setStyleSheet(requiredFieldEmptyStyle);
}
} // namespace sprint_timer::ui::qt_gui
