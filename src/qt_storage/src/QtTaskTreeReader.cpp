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
#include "qt_storage/QtTaskTreeReader.h"
#include "core/utils/StringUtils.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/utils/QueryUtils.h"
#include <fstream>

#include <iostream>

namespace sprint_timer::storage::qt_storage {

QtTaskTreeReader::QtTaskTreeReader(std::filesystem::path storageDir_)
    : storageDir{std::move(storageDir_)}
{
}

TaskMetadataTree QtTaskTreeReader::readTree() const
{
    using entry_t = TaskMetadataTree::entry_t;
    std::vector<std::optional<entry_t>> flattenedTree;

    {
        std::ifstream in{storageDir / "task_tree"};

        if (!in.is_open()) {
            std::cout << "Unable to open file for reading: "
                      << storageDir / "tasktree" << std::endl;
            throw std::runtime_error{"Unable to open file for reading"};
        }

        for (std::string line; std::getline(in, line);) {
            if (line.front() == '*') {
                flattenedTree.emplace_back(std::nullopt);
                continue;
            }

            const auto parts = utils::split(line, ',');
            if (parts.size() != 2) {
                throw std::runtime_error("TODO throw proper error");
            }
            const std::string uuid{parts[0]};
            const auto type =
                static_cast<TaskType>(std::stoi(std::string{parts[1]}));

            flattenedTree.emplace_back(
                std::make_pair(uuid, TaskMetadata{uuid, type}));
        }
    }

    return TaskMetadataTree::unflatten(flattenedTree);
}

} // namespace sprint_timer::storage::qt_storage

