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
#include "external_io/TaskToCsvAlgorithm.h"
#include "cpp_utils/algorithms/string_ext.h"

namespace {

constexpr std::string_view date_format{"hh:mm dd.MM.yyyy"};

} // namespace

namespace sprint_timer::external_io {

std::vector<std::string>
TaskToCsvAlgorithm::toRecords(const api::TaskDTO& task) const
{
    std::vector<std::string> records;
    const auto& tags = task.tags;
    records.emplace_back(task.uuid);
    records.emplace_back(task.name);
    records.emplace_back(alg::join(cbegin(tags), cend(tags), ","));
    records.emplace_back(std::to_string(task.sprints.size()));
    records.emplace_back(std::to_string(task.expectedCost));
    records.emplace_back(std::to_string(task.finished ? 1 : 0));
    records.emplace_back(dw::to_string(task.modificationStamp, date_format));
    records.emplace_back(
        task.notes.transform([&](const auto& note) { return note.text; })
            .value_or(""));
    records.emplace_back(dw::to_string(task.timeFrame.start.value(), date_format));
    records.emplace_back(task.timeFrame.due
                             .transform([](auto dateTime) {
                                 return dw::to_string(dateTime, date_format);
                             })
                             .value_or(""));
    records.emplace_back(task.timeFrame.remindAt
                             .transform([](auto dateTime) {
                                 return dw::to_string(dateTime, date_format);
                             })
                             .value_or(""));
    records.emplace_back(task.timeFrame.recurrence.value_or(""));
    records.emplace_back(std::to_string(static_cast<int>(task.kind)));
    return records;
}

} // namespace sprint_timer::external_io

