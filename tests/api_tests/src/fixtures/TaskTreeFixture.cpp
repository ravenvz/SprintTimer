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
#include "api_tests/fixtures/TaskTreeFixture.h"
#include <optional>

namespace fixtures {

using asp::CommandHandler;
using sprint_timer::api::CreateTaskCommand;
using sprint_timer::api::NoteDTO;
using sprint_timer::api::RegisterSprintBulkCommand;
using sprint_timer::api::TaskDTO;
using sprint_timer::api::TaskTimeframeDTO;
using sprint_timer::api::TaskTreeDTO;
using sprint_timer::api::TaskTypeDTO;

auto uuid_projection = [](const auto& node) { return node.uuid; };

auto givenSomeTaskTreeCreated(
    CommandHandler<CreateTaskCommand>& createTaskHandler,
    CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler)
    -> TaskTreeDTO
{
    using Tags = std::vector<std::string>;
    using namespace dw;
    using namespace std::chrono_literals;

    /*
     *    folder1
     *        project1
     *            task1
     *            task3
     *            project2
     *    task2
     *
     *
     */

    // Fake uuid = 0
    createTaskHandler.handle(CreateTaskCommand{"folder1",
                                               Tags{},
                                               0,
                                               TaskTypeDTO::Folder,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    // Fake uuid = 1
    createTaskHandler.handle(CreateTaskCommand{"project1",
                                               Tags{"ProjectTag1"},
                                               5,
                                               TaskTypeDTO::Project,
                                               "0",
                                               std::nullopt,
                                               NoteDTO{"Task note"},
                                               TaskTimeframeDTO{}});
    // Fake uuid = 2
    createTaskHandler.handle(CreateTaskCommand{"task1",
                                               Tags{"Tag1", "Tag2"},
                                               7,
                                               TaskTypeDTO::Regular,
                                               "1",
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    // Fake uuid = 3
    createTaskHandler.handle(CreateTaskCommand{"project2",
                                               Tags{},
                                               20,
                                               TaskTypeDTO::Project,
                                               "1",
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    // Fake uuid = 4
    createTaskHandler.handle(CreateTaskCommand{"task2",
                                               Tags{"Tag1"},
                                               10,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    // Fake uuid = 5
    createTaskHandler.handle(CreateTaskCommand{"task3",
                                               Tags{"Tag1", "Tag3"},
                                               4,
                                               TaskTypeDTO::Regular,
                                               "1",
                                               1,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    const dw::Date someDate{Year{2021}, Month{3}, Day{3}};
    const dw::DateTime firstSprintStartTime{DateTime{someDate} + 3h};
    const dw::DateTimeRange firstSprintRange{firstSprintStartTime,
                                             firstSprintStartTime + 25min};
    const std::vector<DateTimeRange> intervals{
        firstSprintRange,
        add_offset(firstSprintRange, 1h + 25min),
        add_offset(firstSprintRange, 5h + 50min),
        add_offset(firstSprintRange, 7h + 75min)};

    registerSprintsHandler.handle(RegisterSprintBulkCommand{"5", intervals});

    TaskTreeDTO tree;
    tree.insert(tree.end(),
                TaskDTO{"0",
                        Tags{},
                        "folder1",
                        0,
                        std::vector<dw::DateTimeRange>{},
                        false,
                        dw::current_date_time_local(),
                        std::nullopt,
                        TaskTimeframeDTO{},
                        TaskTypeDTO::Folder});
    tree.insert(std::ranges::find(tree, "0", uuid_projection),
                TaskDTO{"1",
                        Tags{"ProjectTag1"},
                        "project1",
                        5,
                        std::vector<dw::DateTimeRange>{},
                        false,
                        dw::current_date_time_local(),
                        NoteDTO{"Task note"},
                        TaskTimeframeDTO{},
                        TaskTypeDTO::Project});
    tree.insert(std::ranges::find(tree, "1", uuid_projection),
                TaskDTO{"2",
                        Tags{"Tag1", "Tag2"},
                        "task1",
                        7,
                        std::vector<dw::DateTimeRange>{},
                        false,
                        dw::current_date_time_local(),
                        std::nullopt,
                        TaskTimeframeDTO{},
                        TaskTypeDTO::Regular});
    tree.insert(std::ranges::find(tree, "1", uuid_projection),
                TaskDTO{"3",
                        Tags{},
                        "project2",
                        20,
                        std::vector<dw::DateTimeRange>{},
                        false,
                        dw::current_date_time_local(),
                        std::nullopt,
                        TaskTimeframeDTO{},
                        TaskTypeDTO::Project});
    tree.insert(tree.end(),
                TaskDTO{"4",
                        Tags{"Tag1"},
                        "task2",
                        10,
                        std::vector<dw::DateTimeRange>{},
                        false,
                        dw::current_date_time_local(),
                        std::nullopt,
                        TaskTimeframeDTO{},
                        TaskTypeDTO::Regular});
    tree.insert(std::ranges::find(tree, "1", uuid_projection),
                TaskDTO{"5",
                        Tags{"Tag1", "Tag3"},
                        "task3",
                        4,
                        intervals,
                        false,
                        dw::current_date_time_local(),
                        std::nullopt,
                        TaskTimeframeDTO{},
                        TaskTypeDTO::Regular},
                ds::DestinationPosition{1});

    return tree;
}

auto givenTaskTreeWithDueDatesCreated(
    CommandHandler<CreateTaskCommand>& createTaskHandler,
    CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler)
    -> TaskTreeDTO
{
    using Tags = std::vector<std::string>;
    using namespace dw;
    using namespace std::chrono_literals;

    DateTime referenceTime = DateTime{Date{Year{2023}, Month{6}, Day{19}}} + 4h;

    /*
     * folder1
     *    project1
     *       task1 // Due date is set
     *       task2
     *          task3
     *
     * task4
     * */
    createTaskHandler.handle(CreateTaskCommand{"folder1",
                                               Tags{},
                                               0,
                                               TaskTypeDTO::Folder,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    createTaskHandler.handle(CreateTaskCommand{"project1",
                                               Tags{},
                                               30,
                                               TaskTypeDTO::Project,
                                               "0",
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    createTaskHandler.handle(CreateTaskCommand{
        "task1",
        Tags{},
        5,
        TaskTypeDTO::Regular,
        "1",
        std::nullopt,
        std::nullopt,
        TaskTimeframeDTO{
            referenceTime, referenceTime + Days{10}, std::nullopt}});
    createTaskHandler.handle(CreateTaskCommand{"task2",
                                               Tags{},
                                               15,
                                               TaskTypeDTO::Regular,
                                               "1",
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    createTaskHandler.handle(CreateTaskCommand{"task3",
                                               Tags{},
                                               10,
                                               TaskTypeDTO::Regular,
                                               "3",
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    createTaskHandler.handle(CreateTaskCommand{"task4",
                                               Tags{},
                                               7,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    TaskTreeDTO tree;
    tree.insert(tree.end(),
                TaskDTO{"0",
                        Tags{},
                        "folder1",
                        0,
                        std::vector<DateTimeRange>{},
                        false,
                        dw::current_date_time_local(),
                        std::nullopt,
                        TaskTimeframeDTO{},
                        TaskTypeDTO::Folder});
    tree.insert(std::ranges::find(tree, "0", uuid_projection),
                TaskDTO{"1",
                        Tags{},
                        "project1",
                        30,
                        std::vector<DateTimeRange>{},
                        false,
                        dw::current_date_time_local(),
                        std::nullopt,
                        TaskTimeframeDTO{},
                        TaskTypeDTO::Project});
    tree.insert(std::ranges::find(tree, "1", uuid_projection),
                TaskDTO{"2",
                        Tags{},
                        "task1",
                        5,
                        std::vector<DateTimeRange>{},
                        false,
                        dw::current_date_time_local(),
                        std::nullopt,
                        TaskTimeframeDTO{referenceTime,
                                         referenceTime + Days{10},
                                         std::nullopt,
                                         std::nullopt},
                        TaskTypeDTO::Regular});
    tree.insert(std::ranges::find(tree, "1", uuid_projection),
                TaskDTO{"3",
                        Tags{},
                        "task2",
                        15,
                        std::vector<DateTimeRange>{},
                        false,
                        dw::current_date_time_local(),
                        std::nullopt,
                        TaskTimeframeDTO{},
                        TaskTypeDTO::Regular});
    tree.insert(std::ranges::find(tree, "3", uuid_projection),
                TaskDTO{"4",
                        Tags{},
                        "task3",
                        10,
                        std::vector<DateTimeRange>{},
                        false,
                        dw::current_date_time_local(),
                        std::nullopt,
                        TaskTimeframeDTO{},
                        TaskTypeDTO::Regular});
    tree.insert(tree.end(),
                TaskDTO{"5",
                        Tags{},
                        "task4",
                        7,
                        std::vector<DateTimeRange>{},
                        false,
                        dw::current_date_time_local(),
                        std::nullopt,
                        TaskTimeframeDTO{},
                        TaskTypeDTO::Regular});

    return tree;
}

} // namespace fixtures
