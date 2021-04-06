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
#ifndef HISTORYCONTRACT_H_6VK9MSI7
#define HISTORYCONTRACT_H_6VK9MSI7

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include <date_wrapper/date_wrapper.h>
#include <vector>

namespace sprint_timer::ui::contracts::HistoryContract {

struct Item {
    std::string description;
    std::string uuid;
    bool operator==(const Item&) const = default;
};

struct DayHistory {
    dw::Date date;
    std::vector<Item> sortedItems;
    bool operator==(const DayHistory&) const = default;
};

struct History {
    size_t totalItems{0};
    std::vector<DayHistory> sortedDayHistory;
    bool operator==(const History& other) const = default;
};

struct TaskEditData {
    std::string name;
    int estimatedCost;
    std::vector<std::string> tags;
    bool operator==(const TaskEditData&) const = default;
};

struct SprintEditData {
    dw::DateTimeRange range;
    bool operator==(const SprintEditData&) const = default;
};

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
    virtual void onEditTaskMenuSelected(const std::string& uuid) const = 0;

    virtual void onEditSprintMenuSelected(const std::string& uuid) const = 0;

    virtual void onDisplayedTabChanged(int tabNumber) = 0;
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual void displayHistory(const History& history) = 0;

    virtual TaskEditData openEditTaskDialog(const TaskEditData& data) = 0;

    virtual SprintEditData openEditSprintDialog(const SprintEditData& data) = 0;
};

} // namespace sprint_timer::ui::contracts::HistoryContract

#endif /* end of include guard: HISTORYCONTRACT_H_6VK9MSI7 */
