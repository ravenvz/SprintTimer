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
#include "qt_gui/presentation/HistoryPresenter.h"

#include <iostream>

namespace {

/* Precondition: entities are sorted by their time in ascending order. */
template <typename Entity>
sprint_timer::ui::contracts::HistoryContract::History
toHistory(const std::vector<Entity>& entities);

dw::Date extractDate(const sprint_timer::entities::Sprint& sprint);

} // namespace

namespace sprint_timer::ui {

HistoryPresenter::HistoryPresenter(
    SprintsRequestHandler& requestSprintsHandler_,
    TaskRequestHandler& requestTasksHandler_,
    HistoryMediator& mediator_)
    : requestSprintsHandler{requestSprintsHandler_}
    , requestTasksHandler{requestTasksHandler_}
    , mediator{mediator_}
{
    mediator.addColleague(this);
}

HistoryPresenter::~HistoryPresenter() { mediator.removeColleague(this); }

void HistoryPresenter::onEditTaskMenuSelected(const std::string& /*uuid*/) const
{
}

void HistoryPresenter::onEditSprintMenuSelected(
    const std::string& /*uuid*/) const
{
}

void HistoryPresenter::updateViewImpl()
{
    const auto range = mediator.currentDateRange();
    auto v = view();
    if (!v || !range) {
        return;
    }

    const auto history = mediator.displayedHistory();
    switch (history) {
    case HistoryMediator::DisplayedHistory::SprintHistory: {
        const auto sprints = requestSprintsHandler.handle(
            use_cases::RequestSprintsQuery{*range});
        v.value()->displayHistory(toHistory(sprints));
        break;
    }
    case HistoryMediator::DisplayedHistory::TaskHistory: {
        const auto tasks =
            requestTasksHandler.handle(use_cases::FinishedTasksQuery{*range});
        v.value()->displayHistory(toHistory(tasks));
        break;
    }
    }
}

void HistoryPresenter::onViewAttached() { updateView(); }

void HistoryPresenter::onDisplayedTabChanged(int tabNumber)
{
    mediator.changeDisplayedHistory(
        this, static_cast<HistoryMediator::DisplayedHistory>(tabNumber));
}

void HistoryPresenter::onSharedDataChanged() { updateView(); }

} // namespace sprint_timer::ui

namespace {

std::string describe(const sprint_timer::entities::Sprint& sprint)
{
    std::stringstream ss;
    ss << dw::to_string(sprint.startTime(), "hh:mm") << " - "
       << dw::to_string(sprint.finishTime(), "hh:mm") << " ";
    for (const auto& tag : sprint.tags())
        ss << "#" << tag << " ";
    ss << sprint.name();
    std::cout << ss.str() << std::endl;
    return ss.str();
}

std::string describe(const sprint_timer::entities::Task& task)
{
    std::stringstream ss;
    for (const auto& tag : task.tags())
        ss << "#" << tag << ' ';
    ss << task.name();
    ss << ' ' << task.actualCost() << '/' << task.estimatedCost();
    return ss.str();
}

template <typename Entity>
sprint_timer::ui::contracts::HistoryContract::Item toItem(const Entity& entity)
{
    return sprint_timer::ui::contracts::HistoryContract::Item{describe(entity),
                                                              entity.uuid()};
};

dw::Date extractDate(const sprint_timer::entities::Sprint& sprint)
{
    return sprint.startTime().date();
};

dw::Date extractDate(const sprint_timer::entities::Task& task)
{
    return task.lastModified().date();
};

/* Precondition: entities are sorted by their time in ascending order. */
template <typename Entity>
sprint_timer::ui::contracts::HistoryContract::History
toHistory(const std::vector<Entity>& entities)
{
    using namespace sprint_timer::ui::contracts::HistoryContract;
    if (entities.empty()) {
        return History{};
    }

    std::vector<DayHistory> sortedDayHistories;
    for (auto sameDateBeg = cbegin(entities); sameDateBeg != cend(entities);) {
        const auto previousDate = extractDate(*sameDateBeg);
        DayHistory dayHistory{extractDate(*sameDateBeg), std::vector<Item>{}};
        auto datesDiffer = [&previousDate](const auto& entity) {
            return extractDate(entity) != previousDate;
        };
        auto sameDateEnd =
            std::find_if(sameDateBeg, cend(entities), datesDiffer);
        std::transform(sameDateBeg,
                       sameDateEnd,
                       std::back_inserter(dayHistory.sortedItems),
                       toItem<Entity>);
        sortedDayHistories.push_back(std::move(dayHistory));
        sameDateBeg = sameDateEnd;
    }

    return History{entities.size(), std::move(sortedDayHistories)};
}

} // namespace
