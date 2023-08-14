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
#ifndef PLANNERCONTRACT_H_FYB5GGKL
#define PLANNERCONTRACT_H_FYB5GGKL

#include "api/dtos/TaskTreeDTO.h"
#include "core/TreeType.h"
#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"

namespace sprint_timer::ui::contracts::PlannerContract {

struct Item {
    std::string description;
    std::string_view foreground;
    std::string_view background;

    friend auto operator==(const Item&, const Item&) -> bool = default;
};

struct Colors {
    std::pair<std::string_view, std::string_view> textColor;
    std::pair<std::string_view, std::string_view> tagColor;
    std::pair<std::string_view, std::string_view> progressColor;
    std::pair<std::string_view, std::string_view> dueDateColor;
    std::pair<std::string_view, std::string_view> reminderColor;

    friend auto operator==(const Colors&, const Colors&) -> bool = default;
};

struct PlannerItem {
    std::string uuid;
    Item name;
    Item tags;
    Item progress;
    dw::DateTime activeSince{dw::current_date_time_local()};
    Item dueDate;
    std::string notes;
    Item reminder;
    bool finished;
    bool recurrent;
    api::TaskTypeDTO type;

    friend auto operator==(const PlannerItem&, const PlannerItem&)
        -> bool = default;
};

using PlannerTree = TreeType<PlannerItem>;

struct TaskParent {
    std::string parent;
    std::optional<int64_t> beforePosition;
};

template <class CharT, class Traits>
auto operator<<(std::basic_ostream<CharT, Traits>& os,
                const PlannerItem& plannerItem)
    -> std::basic_ostream<CharT, Traits>&
{
    auto displayItem = [&](const auto& item) {
        os << "\t" << '"' << item.description << "\" " << item.background << " "
           << item.foreground << '\n';
    };
    os << "PlannerItem{" << plannerItem.uuid << "\n";
    displayItem(plannerItem.name);
    displayItem(plannerItem.tags);
    displayItem(plannerItem.progress);
    os << " start: " << plannerItem.activeSince;
    displayItem(plannerItem.dueDate);
    displayItem(plannerItem.reminder);
    os << '\t' << plannerItem.notes << '\n';
    os << '\t' << plannerItem.finished << '\n';
    os << '\t' << plannerItem.recurrent << '\n';
    os << '\t' << plannerItem.type << '\n';
    os << "}";
    return os;
}

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
    virtual auto moveNodes(const std::optional<std::string>& sourceParent,
                           int64_t sourceRow,
                           int64_t count,
                           const std::optional<std::string>& destinationParent,
                           int64_t destinationChild) -> void = 0;

    virtual auto deleteTask(std::string&& taskUuid) -> void = 0;

    virtual auto changeTaskAdditionContext(std::optional<std::string>&& parent,
                                           bool isSubtask) -> void = 0;

    virtual auto changeTaskEditionContext(const std::string& uuid) -> void = 0;

    virtual auto quickEditTask(std::string&& uuid,
                               std::string&& name,
                               std::vector<std::string>&& tags,
                               int cost) -> void = 0;

    virtual auto toggleTask(const std::string& uuid) -> void = 0;
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual void displayPlanner(const PlannerTree& taskTree) = 0;
};

} // namespace sprint_timer::ui::contracts::PlannerContract

#endif /* end of include guard: PLANNERCONTRACT_H_FYB5GGKL */

