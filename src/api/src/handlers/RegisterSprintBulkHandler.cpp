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
#include "core/SprintConflictException.h"
#include "api/actions/RegisterSprintBulk.h"
#include "core/entities/Sprint.h"
#include "core/utils/Algutils.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>
#include <stdexcept>

namespace {

dw::DateRange unite(dw::DateRange a, dw::DateTimeRange b);

// Precondition: intervals should not be empty
dw::DateRange fittingRange(const std::vector<dw::DateTimeRange>& intervals);

std::vector<sprint_timer::entities::Sprint>
buildSprintsFromIntervals(const std::string& taskUuid,
                          sprint_timer::UUIDGenerator& uuidGenerator,
                          const std::vector<dw::DateTimeRange>& intervals);

bool orderByStartTime(const sprint_timer::entities::Sprint& lhs,
                      const sprint_timer::entities::Sprint& rhs);

void throwIfSprintConflictDetected(
    const std::vector<sprint_timer::entities::Sprint>& sprints,
    const std::vector<sprint_timer::entities::Sprint>& existingSprints);

bool intersecting(const std::pair<sprint_timer::entities::Sprint,
                                  sprint_timer::entities::Sprint>& sprintPair);

constexpr auto adjacent_view = [](auto&& range) {
    if (range.size() < 2) {
        throw std::runtime_error("adjacent_view should only adapt ranges that "
                                 "have at least 2 elements");
    }
    return std::views::iota(1u, range.size()) |
           std::views::transform(
               [&](auto i) { return std::make_pair(range[i - 1], range[i]); });
};

} // namespace

namespace sprint_timer::api {

RegisterSprintBulkHandler::RegisterSprintBulkHandler(
    TaskStorageReader& taskReader_,
    SprintStorage& sprintStorage_,
    ActionInvoker& actionInvoker_,
    UUIDGenerator& uuidGenerator_)
    : taskReader{taskReader_}
    , sprintStorage{sprintStorage_}
    , actionInvoker{actionInvoker_}
    , uuidGenerator{uuidGenerator_}
{
}

void RegisterSprintBulkHandler::handle(const RegisterSprintBulkCommand& command)
{

    const auto sprints = buildSprintsFromIntervals(
        command.taskUuid, uuidGenerator, command.intervals);

    if (sprints.empty()) {
        return;
    }

    throwIfTaskDoesNotExist(command.taskUuid);

    const auto range = fittingRange(command.intervals);
    const auto existingSprints = sprintStorage.findByDateRange(range);

    throwIfSprintConflictDetected(sprints, existingSprints);

    actionInvoker.execute(
        std::make_unique<actions::RegisterSprintBulk>(sprintStorage, sprints));
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
    // entities::Task task = tasksMatchingUuid.front();
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

std::vector<sprint_timer::entities::Sprint>
buildSprintsFromIntervals(const std::string& taskUuid,
                          sprint_timer::UUIDGenerator& uuidGenerator,
                          const std::vector<dw::DateTimeRange>& intervals)
{
    using namespace sprint_timer::entities;
    auto to_sprint = [&](const auto& interval) {
        return Sprint{"",
                      interval,
                      std::list<Tag>{},
                      uuidGenerator.generateUUID(),
                      taskUuid};
    };
    std::vector<Sprint> sprints;
    sprints.reserve(intervals.size());
    std::ranges::copy(std::ranges::views::transform(intervals, to_sprint),
                      std::back_inserter(sprints));
    std::ranges::sort(sprints, orderByStartTime);
    return sprints;
}

bool orderByStartTime(const sprint_timer::entities::Sprint& lhs,
                      const sprint_timer::entities::Sprint& rhs)
{
    return lhs.startTime() < rhs.startTime();
};

void throwIfSprintConflictDetected(
    const std::vector<sprint_timer::entities::Sprint>& sprints,
    const std::vector<sprint_timer::entities::Sprint>& existingSprints)
{
    using namespace sprint_timer::entities;

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

bool intersecting(const std::pair<sprint_timer::entities::Sprint,
                                  sprint_timer::entities::Sprint>& sprintPair)
{
    return sprint_timer::entities::intersectingInTime(sprintPair.first,
                                                      sprintPair.second);
};

} // namespace

