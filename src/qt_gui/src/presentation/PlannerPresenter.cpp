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
#include "qt_gui/presentation/PlannerPresenter.h"
#include "cpp_utils/algorithms/optional_ext.h"
#include "cpp_utils/algorithms/string_ext.h"
#include <format>

namespace {

using sprint_timer::api::NoteDTO;
using sprint_timer::api::TaskDTO;
using sprint_timer::api::TaskTimeframeDTO;
using sprint_timer::api::TaskTypeDTO;
using sprint_timer::ui::PlannerColors;
using sprint_timer::ui::contracts::PlannerContract::Colors;
using sprint_timer::ui::contracts::PlannerContract::Item;
using sprint_timer::ui::contracts::PlannerContract::PlannerItem;
using sprint_timer::ui::contracts::PlannerContract::TaskParent;

using ColorPair = std::pair<std::string_view, std::string_view>;

enum class OverdueStatus {
    Today,
    OverdueSameWeek,
    LaterSameWeek,
    Later,
    Overdue,
};

// auto makeTaskType(int32_t numeric) -> TaskTypeDTO;

class MakeItemStrategy {
public:
    MakeItemStrategy(const sprint_timer::api::TaskTreeDTO& taskTree_,
                     const PlannerColors& colors_,
                     dw::DateTime now_);

    virtual ~MakeItemStrategy() = default;

    virtual auto makeName(std::string taskName) const -> Item;

    virtual auto makeProgress(const TaskDTO& task) const -> Item;

    auto makeTags(std::string tags) const -> Item;

    auto makeStartDate(const std::optional<TaskTimeframeDTO>& maybeFrame) const
        -> Item;

    auto makeDueDate(const std::optional<TaskTimeframeDTO>& maybeFrame) const
        -> Item;

    auto makeReminder(const std::optional<TaskTimeframeDTO>& maybeFrame) const
        -> Item;

    auto makeNote(const std::optional<NoteDTO>& note) const -> std::string;

protected:
    const sprint_timer::api::TaskTreeDTO& taskTree;
    const PlannerColors& colors;
    dw::DateTime now;
};

class MakeRegularStrategy : public MakeItemStrategy {
public:
    MakeRegularStrategy(const sprint_timer::api::TaskTreeDTO& taskTree_,
                        const PlannerColors& colors_,
                        dw::DateTime now_);

    auto makeProgress(const TaskDTO& task) const -> Item override;
};

class MakeProjectStrategy : public MakeItemStrategy {
public:
    MakeProjectStrategy(const sprint_timer::api::TaskTreeDTO& taskTree_,
                        const PlannerColors& colors_,
                        dw::DateTime now_);

    auto makeName(std::string taskName) const -> Item override;

    auto makeProgress(const TaskDTO& task) const -> Item override;
};

class MakeFolderStrategy : public MakeItemStrategy {
public:
    MakeFolderStrategy(const sprint_timer::api::TaskTreeDTO& taskTree_,
                       const PlannerColors& colors_,
                       dw::DateTime now_);

    auto makeProgress(const TaskDTO& task) const -> Item override;
};

class MakeItem {
public:
    explicit MakeItem(std::span<std::unique_ptr<MakeItemStrategy>> strategies_)
        : strategies{strategies_}
    {
    }

    auto operator()(const TaskDTO& task) const -> PlannerItem
    {
        const auto& strategy = strategies[static_cast<size_t>(task.kind)];
        const auto& taskTags = task.tags;
        const auto tags = alg::join(cbegin(taskTags), cend(taskTags), ", ");
        return PlannerItem{
            task.uuid,
            strategy->makeName(task.name),
            strategy->makeTags(tags),
            strategy->makeProgress(task),
            task.timeFrame.start.value_or(dw::current_date_time_local),
            strategy->makeDueDate(task.timeFrame),
            strategy->makeNote(task.notes),
            strategy->makeReminder(task.timeFrame),
            task.finished,
            task.timeFrame.recurrence.has_value(),
            task.kind};
    }

private:
    std::span<std::unique_ptr<MakeItemStrategy>> strategies;
};

auto weekdayToString(dw::DateTime dateTime) -> std::string;

auto overdueStatus(dw::DateTime now, dw::DateTime date) -> OverdueStatus;

// auto humanizeDate(dw::DateTime now, dw::DateTime date) -> std::string;

} // namespace

namespace sprint_timer::ui {

PlannerPresenter::PlannerPresenter(
    PlannerColors colors_,
    TaskTreeFilter& taskTreeFilter_,
    read_planner_handler_t& readPlannerHandler_,
    save_planner_handler_t& savePlannerHandler_,
    delete_task_handler_t& deleteTaskHandler_,
    edit_task_handler_t& editTaskHandler_,
    toggle_task_completed_handler_t& toggleTaskCompletedHandler_,
    AddTaskContext& addTaskContext_,
    EditTaskContext& editTaskContext_,
    const api::DateTimeProvider& timeProvider_)
    : colors{colors_}
    , taskTreeFilter{taskTreeFilter_}
    , readPlannerHandler{readPlannerHandler_}
    , savePlannerHandler{savePlannerHandler_}
    , deleteTaskHandler{deleteTaskHandler_}
    , editTaskHandler{editTaskHandler_}
    , toggleTaskCompletedHandler{toggleTaskCompletedHandler_}
    , addTaskContext{addTaskContext_}
    , editTaskContext{editTaskContext_}
    , timeProvider{timeProvider_}
{
    taskTreeFilter.attach(*this);
}

PlannerPresenter::~PlannerPresenter() { taskTreeFilter.detach(*this); }

void PlannerPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        std::array<std::unique_ptr<MakeItemStrategy>, 3> strategies{
            std::make_unique<MakeProjectStrategy>(
                data, colors, timeProvider.dateTimeLocalNow()),
            std::make_unique<MakeFolderStrategy>(
                data, colors, timeProvider.dateTimeLocalNow()),
            std::make_unique<MakeRegularStrategy>(
                data, colors, timeProvider.dateTimeLocalNow()),
        };
        const auto itemMaker = MakeItem{strategies};
        if (auto filtered = filteredData; filteredData) {
            v.value()->displayPlanner(filteredData->transform(itemMaker));
        }
        else {
            v.value()->displayPlanner(data.transform(itemMaker));
        }
    }
}

void PlannerPresenter::fetchDataImpl()
{
    data = readPlannerHandler.handle(api::ReadTaskTreeQuery{});
    if (taskTreeFilter.currentFilter()) {
        filteredData = taskTreeFilter(data);
    }
}

auto PlannerPresenter::moveNodes(
    const std::optional<std::string>& sourceParent,
    int64_t sourceRow,
    int64_t count,
    const std::optional<std::string>& destinationParent,
    int64_t destinationChild) -> void
{
    data.move_nodes(
        sourceParent
            ? std::ranges::find(data, *sourceParent, &api::TaskDTO::uuid)
            : data.end(),
        ds::SourcePosition{sourceRow},
        ds::Count{count},
        destinationParent
            ? std::ranges::find(data, *destinationParent, &api::TaskDTO::uuid)
            : data.end(),
        ds::DestinationPosition{destinationChild});
    // data.moveNodes(
    //     sourceParent, sourceRow, count, destinationParent, destinationChild);
    updateView();
    savePlannerHandler.handle(api::SaveTaskTreeCommand{data});
}

auto PlannerPresenter::deleteTask(std::string&& uuid) -> void
{
    deleteTaskHandler.handle(api::DeleteTaskCommand{std::move(uuid)});
}

auto PlannerPresenter::changeTaskAdditionContext(
    std::optional<std::string>&& parent, bool isSubtask) -> void
{
    addTaskContext =
        AddTaskContext{std::move(parent),
                       isSubtask ? TaskAddMode::Subtask : TaskAddMode::Sibling};
}

auto PlannerPresenter::changeTaskEditionContext(const std::string& uuid) -> void
{
    auto it = std::ranges::find(data, uuid, &TaskDTO::uuid);
    if (it == data.cend()) {
        throw std::runtime_error{
            std::format("Cannot find task with uuid: {}", uuid)};
    }
    auto task = *it;
    editTaskContext = EditTaskContext{std::move(task)};
}

auto PlannerPresenter::quickEditTask(std::string&& uuid,
                                     std::string&& name,
                                     std::vector<std::string>&& tags,
                                     int cost) -> void
{
    auto it = std::ranges::find(data, uuid, &TaskDTO::uuid);
    if (it == data.cend()) {
        throw std::runtime_error{
            std::format("Cannot find task with uuid: {}", uuid)};
    }

    auto editedTask = *it;
    editedTask.name = std::move(name);
    editedTask.tags = std::move(tags);
    editedTask.expectedCost = cost;
    editTaskHandler.handle(api::EditTaskCommand{std::move(editedTask)});
}

auto PlannerPresenter::toggleTask(const std::string& uuid) -> void
{
    if (std::ranges::find(data, uuid, &TaskDTO::uuid) != data.cend()) {
        toggleTaskCompletedHandler.handle(
            api::ToggleTaskCompletedCommand{uuid});
    }
}

// auto PlannerPresenter::installFilter(FilterFn filter) -> void
// {
//     dataFilter = filter;
//     data = dataFilter.value()(data);
//     updateView();
// }

auto PlannerPresenter::update() -> void
{
    filteredData = taskTreeFilter.currentFilter()
                       ? taskTreeFilter(data)
                       : std::optional<api::TaskTreeDTO>{};
    updateView();
}

} // namespace sprint_timer::ui

namespace {

MakeItemStrategy::MakeItemStrategy(
    const sprint_timer::api::TaskTreeDTO& taskTree_,
    const PlannerColors& colors_,
    dw::DateTime now_)
    : taskTree{taskTree_}
    , colors{colors_}
    , now{now_}
{
}

auto MakeItemStrategy::makeName(std::string taskName) const -> Item
{
    return {std::move(taskName), colors.text, colors.defaultBackround};
}

auto MakeItemStrategy::makeTags(std::string tags) const -> Item
{
    return {std::move(tags), colors.tag, colors.defaultBackround};
}

auto MakeItemStrategy::makeProgress(const TaskDTO& /* taskNode */) const -> Item
{
    return {"", colors.text, colors.defaultBackround};
}

auto MakeItemStrategy::makeDueDate(
    const std::optional<TaskTimeframeDTO>& maybeFrame) const -> Item
{
    auto formatDuration = [&](dw::DateTime rightNow, dw::DateTime date) {
        return std::string{rightNow < date ? "+" : "-"} +
               std::to_string(dw::DateRange{rightNow.date(), date.date()}
                                  .duration()
                                  .count()) +
               std::string{" Days"};
    };

    return maybeFrame.and_then([](const auto& frame) { return frame.due; })
        .transform([&](const auto& dueDate) -> Item {
            const auto status = overdueStatus(now, dueDate);
            using enum OverdueStatus;
            switch (status) {
            case Today:
                return {"Today", colors.contrastText, colors.doneWork};
            case OverdueSameWeek:
                return {weekdayToString(dueDate),
                        colors.contrastText,
                        colors.dueOverdue};
            case LaterSameWeek:
                return {weekdayToString(dueDate),
                        colors.contrastText,
                        colors.dueSoon};
            case Later:
                return {formatDuration(now, dueDate),
                        colors.contrastText,
                        colors.dueNotSoon};
            case Overdue:
                return {formatDuration(now, dueDate),
                        colors.contrastText,
                        colors.dueOverdue};
            }
            std::unreachable();
        })
        .value_or(Item{"", colors.text, colors.defaultBackround});
}

auto MakeItemStrategy::makeReminder(
    const std::optional<TaskTimeframeDTO>& maybeFrame) const -> Item
{
    const auto dateRepr =
        maybeFrame.and_then([](const auto& frame) { return frame.remindAt; })
            .transform([](const auto& reminder) {
                return dw::to_string(reminder, "hh:mm dd.MM.yyyy");
            });
    return {dateRepr.value_or(""), colors.text, colors.defaultBackround};
}

auto MakeItemStrategy::makeNote(const std::optional<NoteDTO>& maybeNote) const
    -> std::string
{
    return maybeNote.transform([](const auto& note) { return note.text; })
        .value_or("");
}

MakeRegularStrategy::MakeRegularStrategy(
    const sprint_timer::api::TaskTreeDTO& taskTree_,
    const PlannerColors& colors_,
    dw::DateTime now_)
    : MakeItemStrategy{taskTree_, colors_, now_}
{
}

auto MakeRegularStrategy::makeProgress(const TaskDTO& task) const -> Item
{
    const auto expectedCost = task.expectedCost;
    const auto actualCost = static_cast<int>(task.sprints.size());
    const std::string description =
        std::to_string(actualCost) + "/" + std::to_string(expectedCost);
    auto partial = [&](std::string_view textColor) {
        return Item{description, textColor, colors.defaultBackround};
    };
    if (expectedCost == actualCost) {
        return partial(colors.doneWork);
    }
    if (actualCost > expectedCost) {
        return partial(colors.overworkWork);
    }
    return partial(colors.text);
}

MakeProjectStrategy::MakeProjectStrategy(
    const sprint_timer::api::TaskTreeDTO& taskTree_,
    const PlannerColors& colors_,
    dw::DateTime now_)
    : MakeItemStrategy{taskTree_, colors_, now_}
{
}

auto MakeProjectStrategy::makeName(std::string taskName) const -> Item
{
    return {std::move(taskName), colors.tag, colors.defaultBackround};
}

auto MakeProjectStrategy::makeProgress(const TaskDTO& /* task */) const -> Item
{
    // TODO implement
    return {"", colors.text, colors.defaultBackround};
}

MakeFolderStrategy::MakeFolderStrategy(
    const sprint_timer::api::TaskTreeDTO& taskTree_,
    const PlannerColors& colors_,
    dw::DateTime now_)
    : MakeItemStrategy{taskTree_, colors_, now_}
{
}

auto MakeFolderStrategy::makeProgress(const TaskDTO& /* taskNode */) const
    -> Item
{
    return {"", colors.text, colors.defaultBackround};
}

auto overdueStatus(dw::DateTime now, dw::DateTime date) -> OverdueStatus
{
    if (now.date() == date.date()) {
        return OverdueStatus::Today;
    }
    const auto isSameWeek =
        dw::IsoDate{now}.weeknum() == dw::IsoDate{date}.weeknum();
    if (now.date() < date.date()) {
        return isSameWeek ? OverdueStatus::LaterSameWeek : OverdueStatus::Later;
    }
    return isSameWeek ? OverdueStatus::OverdueSameWeek : OverdueStatus::Overdue;
}

auto weekdayToString(dw::DateTime dateTime) -> std::string
{
    using enum dw::Weekday;
    switch (dateTime.weekday()) {
    case Monday:
        return "Monday";
    case Tuesday:
        return "Tuesday";
    case Wednesday:
        return "Wednesday";
    case Thursday:
        return "Thursday";
    case Friday:
        return "Friday";
    case Saturday:
        return "Saturday";
    case Sunday:
        return "Sunday";
    }
    // TODO replace with std::unreachable
    throw std::runtime_error{"Unreachable"};
};

// auto humanizeDate(dw::DateTime now, dw::DateTime date) -> std::string
// {
//     const auto days = dw::DateRange{now.date(),
//     date.date()}.duration().count(); if (now.date() == date.date()) {
//         return "Today";
//     }
//     if (now.date() < date.date()) {
//         return std::string{"-"} + std::to_string(days) + std::string{"
//         Days"};
//     }
//     // days > 0 here
//     if (dw::IsoDate{now}.weeknum() == dw::IsoDate{date}.weeknum()) {
//         return weekdayToString(date);
//     }
//     return std::string{now.date() < date.date() ? "+" : "-"} +
//            std::to_string(days) + std::string{" Days"};
// }

// auto makeTaskType(int32_t numeric) -> TaskTypeDTO
// {
//     switch (numeric) {
//     case static_cast<int32_t>(TaskTypeDTO::Regular):
//     case static_cast<int32_t>(TaskTypeDTO::Project):
//     case static_cast<int32_t>(TaskTypeDTO::Folder):
//         return static_cast<TaskTypeDTO>(numeric);
//     default:
//         throw std::runtime_error{"TaskTypeDTO enum is out of range"};
//     }
// }

} // namespace
