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
#include "api/ActionInvoker.h"
#include "api/SprintStorageReader.h"
#include "api/TaskStorage.h"
#include "api/actions/RegisterSprintBulk.h"
#include "api/dtos/SprintMapper.h"
#include "core/SprintConflictException.h"
#include "cpp_utils/patterns/Converter.h"
#include <algorithm>
#include <format>
#include <numeric>
#include <ranges>
#include <sstream>
#include <stdexcept>

namespace {

using sprint_timer::Sprint;

constexpr auto
to_date_range(const dw::DateTimeRange& dateTimeRange) noexcept -> dw::DateRange;

auto to_sprint(const dw::DateTimeRange& dateTimeRange) -> Sprint;

constexpr auto unite(dw::DateRange a,
                     dw::DateRange b) noexcept -> dw::DateRange;

// Precondition: intervals should not be empty
auto fittingRange(const std::vector<dw::DateTimeRange>& intervals)
    -> dw::DateRange;

auto orderByStartTime(const Sprint& lhs, const Sprint& rhs) -> bool;

auto intersecting(const std::tuple<Sprint, Sprint>& sprintPair) -> bool;

auto throwIfSprintConflictDetected(auto&& sprints,
                                   auto&& existingSprints) -> void
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

    auto conflicting_view = mergedSprints | std::views::adjacent<2> |
                            std::views::filter(intersecting);

    if (not conflicting_view.empty()) {
        throw SprintConflictException{
            conflicting_view |
            std::ranges::to<std::vector<
                SprintConflictException::conflicting_sprints_pair>>()};
    }
}

} // namespace

namespace sprint_timer::api {

RegisterSprintBulkHandler::RegisterSprintBulkHandler(
    TaskStorage& taskStorage_,
    SprintStorage& sprintStorage_,
    DateTimeProvider& dateTimeProvider_,
    ActionInvoker& actionInvoker_)
    : taskStorage{taskStorage_}
    , sprintStorage{sprintStorage_}
    , dateTimeProvider{dateTimeProvider_}
    , actionInvoker{actionInvoker_}
{
}

auto RegisterSprintBulkHandler::handle(const RegisterSprintBulkCommand& command)
    -> void
{
    if (command.intervals.empty()) {
        return;
    }

    throwIfTaskDoesNotExist(command.taskUuid);

    const std::vector<Sprint> sprints = command.intervals |
                                        std::views::transform(to_sprint) |
                                        std::ranges::to<std::vector>();

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
        actions::RegisterSprintBulk{taskStorage,
                                    sprintStorage,
                                    dateTimeProvider.dateTimeLocalNow(),
                                    command.taskUuid,
                                    sprints});
}

auto RegisterSprintBulkHandler::throwIfTaskDoesNotExist(
    const std::string& taskUuid) -> void
{
    const auto tasksMatchingUuid = taskStorage.findByUuid(taskUuid);
    if (tasksMatchingUuid.empty() ||
        tasksMatchingUuid.front().uuid() != taskUuid) {
        throw SprintTimerException{std::format(
            "Cannot register sprint for task with uuid: {} -- not found",
            taskUuid)};
    }
}

} // namespace sprint_timer::api

namespace {

// Precondition: intervals should not be empty
auto fittingRange(const std::vector<dw::DateTimeRange>& intervals)
    -> dw::DateRange
{
    return std::ranges::fold_left_first(
               intervals | std::views::transform(to_date_range), unite)
        .value();
};

constexpr auto
to_date_range(const dw::DateTimeRange& dateTimeRange) noexcept -> dw::DateRange
{
    return dw::DateRange{dateTimeRange.start().date(),
                         dateTimeRange.finish().date()};
}

auto to_sprint(const dw::DateTimeRange& dateTimeRange) -> Sprint
{
    return Sprint{dateTimeRange};
}

constexpr auto unite(dw::DateRange a, dw::DateRange b) noexcept -> dw::DateRange
{
    return dw::DateRange{std::min(a.start(), b.start()),
                         std::max(a.finish(), b.finish())};
};

auto orderByStartTime(const Sprint& lhs, const Sprint& rhs) -> bool
{
    return lhs.timeSpan().start() < rhs.timeSpan().start();
};

auto intersecting(const std::tuple<Sprint, Sprint>& sprintPair) -> bool
{
    const auto& [left, right] = sprintPair;
    return sprint_timer::intersectingInTime(left, right);
};

} // namespace

