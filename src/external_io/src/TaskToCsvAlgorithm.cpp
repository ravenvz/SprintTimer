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
#include "core/utils/StringUtils.h"

namespace sprint_timer::external_io {

std::vector<std::string>
TaskToCsvAlgorithm::toRecords(const use_cases::TaskDTO& task) const
{
    using sprint_timer::utils::join;
    std::vector<std::string> records;
    const auto& tags = task.tags;
    records.emplace_back(task.uuid);
    records.emplace_back(task.name);
    records.emplace_back(join(cbegin(tags), cend(tags), ","));
    records.emplace_back(std::to_string(task.actualCost));
    records.emplace_back(std::to_string(task.expectedCost));
    records.push_back(std::to_string(task.finished ? 1 : 0));
    records.push_back(
        dw::to_string(task.modificationStamp, "hh:mm dd.MM.yyyy"));
    return records;
}

} // namespace sprint_timer::external_io

