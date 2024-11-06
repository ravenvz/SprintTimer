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
#include "cpp_utils/algorithms/optional_ext.h"
#include "cpp_utils/algorithms/string_ext.h"
#include "qt_gui/dialogs/DateRangePickDialog.h"
#include "qt_gui/presentation/DateRangeSelectorContract.h"
#include <QAbstractItemModel>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>

namespace {

// TODO consider QStringView
const QString requiredFieldEmptyStyle{"QLineEdit { border: 2px solid red; }"};

constexpr int layoutSpacing{25};
constexpr int maxVisibleItems{15};
constexpr std::array<std::string_view, 3> taskTypes{
    "Regular", "Project", "Folder"};

auto makeTypesList() -> QStringList;

auto intFrom(sprint_timer::api::TaskTypeDTO type) -> int;

auto taskTypeFrom(int type) -> sprint_timer::api::TaskTypeDTO;

} // namespace

namespace sprint_timer::ui::qt_gui {

using api::TaskDTO;

TaskDialog::TaskDialog(dw::Weekday firstDayOfWeek_, QWidget* parent_)
    : DisplayableDialog{parent_}
    , firstDayOfWeek{firstDayOfWeek_}
    , name{std::make_unique<QLineEdit>().release()}
    , cost{std::make_unique<QSpinBox>().release()}
    , tagsField{std::make_unique<QLineEdit>().release()}
    , typesModel{makeTypesList()}
    , typesBox{std::make_unique<QComboBox>().release()}
{
    typesBox->setModel(&typesModel);

    auto layout_ = std::make_unique<QFormLayout>();

    name->setPlaceholderText("Task name (required)");

    cost->setMinimum(1);

    auto tagLayout = std::make_unique<QHBoxLayout>();
    auto tagBox_ = std::make_unique<QComboBox>();

    tagBox_->setMaxVisibleItems(maxVisibleItems);
    tagBox_->setEditable(true);
    tagBox_->setModel(&tagModel);
    tagBox_->setPlaceholderText(QStringLiteral("--Select Tag--"));
    connect(
        tagBox_.get(), &QComboBox::textActivated, [this](const QString& tag) {
            QString prevTag = tagsField->text();
            if (!prevTag.isEmpty()) {
                prevTag.append(" ");
            }
            prevTag.append(tag);
            tagsField->setText(prevTag);
        });

    tagLayout->addWidget(tagsField);
    tagLayout->addWidget(tagBox_.release());

    auto mandatoryLayout = std::make_unique<QFormLayout>();
    mandatoryLayout->addRow(name);
    mandatoryLayout->addRow("Estimated cost", cost);
    mandatoryLayout->addRow("Tag", tagLayout.release());
    mandatoryLayout->addRow("Type", typesBox);

    auto mandatoryGroup = std::make_unique<QGroupBox>();
    mandatoryGroup->setLayout(mandatoryLayout.release());

    auto makeDateTimeWidget = []() {
        auto widget = std::make_unique<QDateTimeEdit>();
        widget->setDisplayFormat("dd.MM.yyyy hh:mm");
        widget->setCalendarPopup(true);
        widget->setDate(QDate::currentDate());
        widget->calendarWidget()->setFirstDayOfWeek(Qt::Monday);
        return widget;
    };

    start = makeDateTimeWidget().release();
    due = makeDateTimeWidget().release();
    reminder = makeDateTimeWidget().release();

    auto makeCheckableDateFrame =
        [](const QString& label, QWidget* widget, bool checkable) {
            auto frame = std::make_unique<QGroupBox>(label);
            if (checkable) {
                frame->setCheckable(checkable);
                frame->setChecked(false);
            }
            auto layout = std::make_unique<QHBoxLayout>();
            layout->addWidget(widget);
            frame->setLayout(layout.release());
            return frame;
        };

    auto startFrame = makeCheckableDateFrame("Start", start, false);
    dueFrame = makeCheckableDateFrame("Due", due, true).release();
    reminderFrame =
        makeCheckableDateFrame("Reminder", reminder, true).release();

    recurrence =
        std::make_unique<QLineEdit>().release(); // TODO replace with proper ui

    auto buttons_ = std::make_unique<QDialogButtonBox>(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    notes = std::make_unique<QTextEdit>().release();

    connect(
        buttons_.get(), &QDialogButtonBox::accepted, this, &QDialog::accept);

    connect(
        buttons_.get(), &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(
        name, &QLineEdit::textEdited, [this]() { name->setStyleSheet(""); });

    layout_->addRow(mandatoryGroup.release());
    layout_->addRow(startFrame.release());
    layout_->addRow(dueFrame);
    layout_->addRow(reminderFrame);
    layout_->addRow("Recurrence", recurrence);
    layout_->addRow(notes);
    layout_->setSpacing(layoutSpacing);
    layout_->addRow(buttons_.release());

    setLayout(layout_.release());
}

auto TaskDialog::parseFormFields() const -> TaskDTO
{
    TaskDTO task;
    task.name = name->text().toStdString();
    task.kind = taskTypeFrom(typesBox->currentIndex());
    task.expectedCost = cost->value();
    task.tags.clear();
    const auto tagStr = tagsField->text().toStdString();
    alg::parseWords(
        cbegin(tagStr), cend(tagStr), std::back_inserter(task.tags));
    const auto startTime = dateTimeConverter(start->dateTime());
    const auto dueTime = maybeDateTime(due);
    const auto reminderTime = maybeDateTime(reminder);
    const auto recurrenceText = recurrence->text().trimmed();
    const auto recurrenceStr = recurrenceText.isEmpty()
                                   ? std::optional<std::string>{}
                                   : recurrenceText.toStdString();

    const auto timeFrame =
        api::TaskTimeframeDTO{startTime, dueTime, reminderTime, recurrenceStr};

    const auto notesText = notes->toPlainText().trimmed();

    task.notes = notesText.isEmpty() ? std::optional<api::NoteDTO>{}
                                     : api::NoteDTO{notesText.toStdString()};
    task.timeFrame = timeFrame;

    return task;
}

auto TaskDialog::fillFormFields(const TaskDTO& task) -> void
{
    typesBox->setCurrentIndex(intFrom(task.kind));
    name->setText(QString::fromStdString(task.name));
    cost->setValue(task.expectedCost);
    QString joined_tags = QString::fromStdString(
        alg::join(task.tags.cbegin(), task.tags.cend(), " "));
    tagsField->setText(joined_tags);
    const auto& frame = task.timeFrame;
    start->setDateTime(dateTimeConverter(frame.start.value()));
    alg::inspect(frame.due, [this](const auto& dateTime) {
        dueFrame->setChecked(true);
        due->setDateTime(dateTimeConverter(dateTime));
    });
    alg::inspect(frame.remindAt, [this](const auto& dateTime) {
        reminderFrame->setChecked(true);
        reminder->setDateTime(dateTimeConverter(dateTime));
    });
    alg::inspect(frame.recurrence, [this](const auto& recStr) {
        recurrence->setText(QString::fromStdString(recStr));
    });
    alg::inspect(task.notes, [this](const auto& note) {
        notes->setText(QString::fromStdString(note.text));
    });
}

auto TaskDialog::fillTagField(std::span<const std::string> tags) -> void
{
    QStringList tagList;
    std::ranges::transform(
        tags, std::back_inserter(tagList), [](const auto& tag) {
            return QString::fromStdString(tag);
        });
    tagModel.setStringList(tagList);
}

auto TaskDialog::nameIsEmpty() const -> bool { return name->text().isEmpty(); }

auto TaskDialog::markNameFieldRed() -> void
{
    name->setStyleSheet(requiredFieldEmptyStyle);
}

auto TaskDialog::maybeDateTime(QDateTimeEdit* widget) const
    -> std::optional<dw::DateTime>
{
    return widget->isEnabled() ? dateTimeConverter(widget->dateTime())
                               : std::optional<dw::DateTime>{};
}

} // namespace sprint_timer::ui::qt_gui

namespace {

auto makeTypesList() -> QStringList
{
    QStringList taskNamesList;
    std::ranges::transform(
        taskTypes, std::back_inserter(taskNamesList), [](auto name) {
            return QString::fromStdString(std::string{name});
        });
    return taskNamesList;
}

auto intFrom(sprint_timer::api::TaskTypeDTO type) -> int
{
    switch (type) {
        using enum sprint_timer::api::TaskTypeDTO;
    case Folder:
        return 2;
    case Project:
        return 1;
    case Regular:
        return 0;
    default:
        throw std::runtime_error{"Not all enumerations handled"};
    }
}

auto taskTypeFrom(int type) -> sprint_timer::api::TaskTypeDTO
{
    switch (type) {
        using enum sprint_timer::api::TaskTypeDTO;
    case 0:
        return Regular;
    case 1:
        return Project;
    case 2:
        return Folder;
    default:
        throw std::runtime_error("Enum out of range");
    }
}

} // namespace
