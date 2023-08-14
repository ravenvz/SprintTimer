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
#include "api/handlers/RegisterSprintBulkHandler.h"
#include "api/actions/RegisterSprintBulk.h"
#include "api/dtos/SprintMapper.h"
#include "core/SprintConflictException.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>
#include <stdexcept>

namespace {

using sprint_timer::Sprint;

dw::DateRange unite(dw::DateRange a, dw::DateTimeRange b);

// Precondition: intervals should not be empty
dw::DateRange fittingRange(const std::vector<dw::DateTimeRange>& intervals);

bool orderByStartTime(const Sprint& lhs, const Sprint& rhs);

// void throwIfSprintConflictDetected(
//     const std::vector<Sprint>& sprints,
//     const std::vector<Sprint>& existingSprints);

bool intersecting(const std::pair<Sprint, Sprint>& sprintPair);

constexpr auto adjacent_view = [](auto&& range) {
    if (range.size() < 2) {
        throw std::runtime_error("adjacent_view should only adapt ranges that "
                                 "have at least 2 elements");
    }
    return std::views::iota(1u, range.size()) |
           std::views::transform(
               [&](auto i) { return std::make_pair(range[i - 1], range[i]); });
};

auto throwIfSprintConflictDetected(auto&& sprints, auto&& existingSprints)
{
    using namespace sprint_timer;

    const auto mergedSize = sprints.size() + existingSprints.size();
    if (mergedSize < 2) {
        return;
    }

    std::vector<Sprint> mergedSprints;
    mergedSprints.reserve(mergedSize);
    std::ranges::merge(sprints,
                       existingSprints,
                       std::back_inserter(mergedSprints),
                       orderByStartTime);

    auto conflicting_view = adjacent_view(mergedSprints) |
                            std::views::filter(intersecting) |
                            std::views::common;

    if (!conflicting_view.empty()) {
        throw sprint_timer::SprintConflictException{
            std::vector<std::pair<Sprint, Sprint>>(conflicting_view.begin(),
                                                   conflicting_view.end())};
    }
}

} // namespace

namespace sprint_timer::api {

RegisterSprintBulkHandler::RegisterSprintBulkHandler(
    TaskStorageReader& taskReader_,
    SprintStorage& sprintStorage_,
    ActionInvoker& actionInvoker_,
    const patterns::Converter<dw::DateTimeRange, Sprint>& sprintMapper_)
    : taskReader{taskReader_}
    , sprintStorage{sprintStorage_}
    , actionInvoker{actionInvoker_}
    , sprintMapper{sprintMapper_}
{
}

void RegisterSprintBulkHandler::handle(const RegisterSprintBulkCommand& command)
{
    std::vector<Sprint> sprints;
    sprints.reserve(command.intervals.size());
    std::ranges::copy(sprintMapper(command.intervals),
                      std::back_inserter(sprints));

    if (sprints.empty()) {
        return;
    }

    throwIfTaskDoesNotExist(command.taskUuid);

    const auto range = fittingRange(command.intervals);
    const auto existingSprints = sprintStorage.findByDateRange(range);

    // TODO make sure that command intervals are sorted. Maybe through command
    // validator
    throwIfSprintConflictDetected(
        std::views::all(sprints),
        std::views::transform(existingSprints, [](const auto& sp) {
            return Sprint{sp.timeSpan()};
        }));

    actionInvoker.execute(
        actions::RegisterSprintBulk{sprintStorage, command.taskUuid, sprints});
}

void RegisterSprintBulkHandler::throwIfTaskDoesNotExist(
    const std::string& taskUuid)
{
    const auto tasksMatchingUuid = taskReader.findByUuid(taskUuid);
    if (tasksMatchingUuid.empty() ||
        tasksMatchingUuid.front().uuid() != taskUuid) {
        std::string msg{"Cannot register sprint for task with uuid: "};
        msg += taskUuid;
        msg += " was not found";
        throw SprintTimerException{msg};
    }
    // Task task = tasksMatchingUuid.front();
    // for (const auto& sprint : sprints) {
    //     task.addSprint(sprint);
    // }
}

} // namespace sprint_timer::api

namespace {

// Precondition: intervals should not be empty
dw::DateRange fittingRange(const std::vector<dw::DateTimeRange>& intervals)
{
    dw::DateRange range = dw::DateRange{intervals.front().start().date(),
                                        intervals.front().finish().date()};
    std::accumulate(cbegin(intervals) + 1, cend(intervals), range, unite);
    return range;
};

dw::DateRange unite(dw::DateRange a, dw::DateTimeRange b)
{
    return dw::DateRange{std::min(a.start(), b.start().date()),
                         std::max(a.finish(), b.finish().date())};
};

bool orderByStartTime(const Sprint& lhs, const Sprint& rhs)
{
    return lhs.timeSpan().start() < rhs.timeSpan().start();
};

bool intersecting(const std::pair<Sprint, Sprint>& sprintPair)
{
    return sprint_timer::intersectingInTime(sprintPair.first,
                                            sprintPair.second);
};

} // namespace

