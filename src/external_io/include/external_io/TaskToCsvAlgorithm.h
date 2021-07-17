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
#ifndef TASKTOCSVALGORITHM_H_1UWREKPC
#define TASKTOCSVALGORITHM_H_1UWREKPC

#include "core/use_cases/TaskDTO.h"
#include "external_io/CsvSerializationAlgorithm.h"

namespace sprint_timer::external_io {

class TaskToCsvAlgorithm
    : public CsvSerializationAlgorithm<use_cases::TaskDTO> {
private:
    std::vector<std::string>
    toRecords(const use_cases::TaskDTO& task) const override;
};

} // namespace sprint_timer::external_io

#endif /* end of include guard: TASKTOCSVALGORITHM_H_1UWREKPC */

