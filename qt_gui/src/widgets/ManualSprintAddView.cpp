/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "qt_gui/widgets/ManualSprintAddView.h"
#include "qt_gui/delegates/SubmissionItemDelegate.h"
#include "qt_gui/metatypes/TaskMetaType.h"
#include "qt_gui/models/ReplaceModel.h"
#include "qt_gui/models/TaskModelRoles.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include <QCalendarWidget>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QStringListModel>

namespace {

using sprint_timer::entities::Task;

void shiftDate(QTimeEdit* timeEdit, const QDate& date);

std::chrono::seconds totalSprintLength(int numSprints,
                                       std::chrono::seconds sprintDuration);

struct SprintCreationData {
    dw::DateTime firstSprintStart{dw::current_date_time()};
    size_t numSprints{0};
    size_t taskNumber{0};
};

class AddSprintDialog : public QDialog {

public:
    AddSprintDialog(const std::vector<Task>& tasks,
                    dw::Weekday firstDayOfWeek,
                    std::chrono::minutes sprintDuration,
                    SprintCreationData& returnData_,
                    QDialog* parent = nullptr);

    ~AddSprintDialog() override;

private:
    std::chrono::minutes sprintDuration;
    QTimeEdit* startTime = std::make_unique<QTimeEdit>().release();
    QTimeEdit* finishTime = std::make_unique<QTimeEdit>().release();
    QSpinBox* sprintNumber = std::make_unique<QSpinBox>().release();
    QTimeEdit* lastChangedTime = startTime;
    sprint_timer::ui::qt_gui::ReplaceModel taskModel;
    sprint_timer::ui::qt_gui::SubmissionItemDelegate delegate;

    void adjustStartTime(const QDateTime& /*dateTime*/);

    void adjustFinishTime(const QDateTime& /*dateTime*/);

    void adjustTime();
};

void fillTaskModel(QAbstractItemModel& taskModel,
                   const std::vector<Task>& tasks);

} // namespace

namespace sprint_timer::qt_gui {

ManualSprintAddView::ManualSprintAddView(
    ui::contracts::ManualSprintAddContract::Presenter& presenter_,
    QWidget* parent_)
    : QPushButton{parent_}
    , presenter{presenter_}
{
    setText("Add Sprint Manually");
    connect(this, &QPushButton::clicked, [this]() {
        presenter.onSprintAddRequested();
    });
    presenter.attachView(*this);
}

ManualSprintAddView::~ManualSprintAddView() { presenter.detachView(*this); }

void ManualSprintAddView::displayAddSprintDialog(
    const std::vector<entities::Task>& activeTasks,
    dw::Weekday firstDayOfWeek,
    std::chrono::minutes sprintDuration)
{
    SprintCreationData data;
    AddSprintDialog dialog{activeTasks, firstDayOfWeek, sprintDuration, data};
    if (dialog.exec() == QDialog::Accepted) {
        presenter.onSprintAddConfirmed(
            data.taskNumber, data.firstSprintStart, data.numSprints);
    }
}

void ManualSprintAddView::setInteractive(bool interactive)
{
    setEnabled(interactive);
}

} // namespace sprint_timer::qt_gui

namespace {

AddSprintDialog::AddSprintDialog(const std::vector<Task>& tasks_,
                                 dw::Weekday firstDayOfWeek_,
                                 std::chrono::minutes sprintDuration_,
                                 SprintCreationData& returnData_,
                                 QDialog* parent_)
    : QDialog{parent_}
    , sprintDuration{sprintDuration_}
{
    auto layout = std::make_unique<QFormLayout>();
    auto datePicker = std::make_unique<QCalendarWidget>();
    auto taskSelector = std::make_unique<QComboBox>();
    auto buttons = std::make_unique<QDialogButtonBox>(QDialogButtonBox::Ok |
                                                      QDialogButtonBox::Cancel);

    datePicker->setFirstDayOfWeek(
        firstDayOfWeek_ == dw::Weekday::Monday ? Qt::Monday : Qt::Sunday);
    datePicker->setMaximumDate(QDate::currentDate());
    sprintNumber->setMinimum(1);
    startTime->setDisplayFormat("hh:mm");
    finishTime->setDisplayFormat("hh:mm");
    finishTime->setDateTime(QDateTime::currentDateTime());

    connect(buttons.get(), &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons.get(), &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(startTime,
            &QTimeEdit::dateTimeChanged,
            this,
            &AddSprintDialog::adjustFinishTime);
    connect(finishTime,
            &QTimeEdit::dateTimeChanged,
            this,
            &AddSprintDialog::adjustStartTime);
    connect(sprintNumber,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &AddSprintDialog::adjustTime);
    connect(
        datePicker.get(), &QCalendarWidget::clicked, [this](const QDate& date) {
            shiftDate(startTime, date);
            shiftDate(finishTime, date);
        });
    connect(this,
            &QDialog::accepted,
            [this, &returnData_, sel = taskSelector.get()]() {
                returnData_ = SprintCreationData{
                    sprint_timer::ui::qt_gui::utils::toDateTime(
                        startTime->dateTime()),
                    static_cast<size_t>(sprintNumber->value()),
                    static_cast<size_t>(sel->currentIndex())};
            });

    fillTaskModel(taskModel, tasks_);

    taskSelector->setModel(&taskModel);
    taskSelector->setItemDelegate(&delegate);

    layout->addRow("Task:", taskSelector.release());
    layout->addRow("Date:", datePicker.release());
    layout->addRow("Start time:", startTime);
    layout->addRow("Finish time:", finishTime);
    layout->addRow("Number of sprints:", sprintNumber);
    layout->addRow(buttons.release());

    adjustStartTime(finishTime->dateTime());

    setLayout(layout.release());
}

AddSprintDialog::~AddSprintDialog() = default;

void AddSprintDialog::adjustFinishTime(const QDateTime& dateTime)
{
    lastChangedTime = startTime;
    const QSignalBlocker blocker{finishTime};
    finishTime->setDateTime(dateTime.addSecs(
        totalSprintLength(sprintNumber->value(), sprintDuration).count()));
}

void AddSprintDialog::adjustStartTime(const QDateTime& dateTime)
{
    lastChangedTime = finishTime;
    const QSignalBlocker blocker{startTime};
    startTime->setDateTime(dateTime.addSecs(
        -totalSprintLength(sprintNumber->value(), sprintDuration).count()));
}

void AddSprintDialog::adjustTime()
{
    lastChangedTime == startTime ? adjustFinishTime(startTime->dateTime())
                                 : adjustStartTime(finishTime->dateTime());
}

void shiftDate(QTimeEdit* timeEdit, const QDate& date)
{
    const QSignalBlocker blocker{timeEdit};
    const auto time = timeEdit->time();
    timeEdit->setDate(date);
    timeEdit->setTime(time);
}

std::chrono::seconds totalSprintLength(int numSprints,
                                       std::chrono::seconds sprintDuration)
{
    return numSprints * sprintDuration;
}

void fillTaskModel(QAbstractItemModel& taskModel,
                   const std::vector<Task>& tasks)
{
    for (const auto& task : tasks) {
        QVariant var;
        var.setValue(task);
        taskModel.insertRow(taskModel.rowCount(QModelIndex{}));
        taskModel.setData(
            taskModel.index(taskModel.rowCount(QModelIndex{}) - 1, 0),
            var,
            static_cast<int>(sprint_timer::ui::qt_gui::TaskModelRoles::Insert));
    }
}

} // namespace
