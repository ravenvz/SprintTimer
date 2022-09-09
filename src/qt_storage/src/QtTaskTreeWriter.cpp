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
#include "qt_storage/QtTaskTreeWriter.h"
#include "core/utils/StringUtils.h"
#include <fstream>
#include <iostream>
#include <utility>

namespace {

constexpr std::string_view taskTreeFileName{"task_tree"};

auto serialize_record(const auto& entry) -> std::string
{
    if (entry) {
        return entry->first;
    }
    return std::string{"|"};
};

} // namespace

namespace sprint_timer::storage::qt_storage {

QtTaskTreeWriter::QtTaskTreeWriter(QString connectionName_,
                                   std::filesystem::path storageDir_)
    : connectionName{std::move(connectionName_)}
    , storageDir{std::move(storageDir_)}
{
}

void QtTaskTreeWriter::saveTree(const TaskMetadataTree& taskTree) const
{
    const auto flattenedTree = taskTree.flatten();
    std::ofstream out{storageDir / taskTreeFileName};
    if (!out.is_open()) {
        std::cout << "Unable to open file for writing: "
                  << storageDir / taskTreeFileName << std::endl;
        return;
    }
    std::cout << "File opened for writing: " << storageDir / taskTreeFileName
              << std::endl;
    for (const auto& node : flattenedTree) {
        if (!node) {
            out << "*\n";
            continue;
        }
        out << node->first << ',';
        out << static_cast<int>(node->second.taskType) << '\n';
    }
}

} // namespace sprint_timer::storage::qt_storage
