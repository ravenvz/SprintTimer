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
#ifndef TASKDTOTREEFIXTURES_H_ALQPXRZG
#define TASKDTOTREEFIXTURES_H_ALQPXRZG

#include "api/dtos/TaskDTO.h"
#include "api/dtos/TaskTimeframeDTO.h"
#include "date_wrapper/date_wrapper.h"

namespace fixtures {

using namespace std::chrono_literals;

struct TaskDtoTreeFixture {

    explicit TaskDtoTreeFixture(dw::DateTime anchorTime_)
        : anchorTime{anchorTime_}
    {
        make_tree();
    }

    auto makeDefaultTimeFrame() const -> sprint_timer::api::TaskTimeframeDTO
    {
        return sprint_timer::api::TaskTimeframeDTO{.start = anchorTime,
                                                   .due = std::nullopt,
                                                   .remindAt = std::nullopt,
                                                   .recurrence = std::nullopt};
    }

    dw::DateTime anchorTime;

    sprint_timer::api::TaskDTO folder1{"f1",
                                       {},
                                       "folder1",
                                       0,
                                       {},
                                       false,
                                       anchorTime - std::chrono::hours{3},
                                       std::nullopt,
                                       makeDefaultTimeFrame(),
                                       sprint_timer::api::TaskTypeDTO::Folder};

    sprint_timer::api::TaskDTO folder2{"f2",
                                       {},
                                       "folder2",
                                       0,
                                       {},
                                       false,
                                       anchorTime - std::chrono::hours{2},
                                       std::nullopt,
                                       makeDefaultTimeFrame(),
                                       sprint_timer::api::TaskTypeDTO::Folder};

    sprint_timer::api::TaskDTO project1{
        "p1",
        {},
        "project1",
        0,
        {},
        false,
        anchorTime - dw::Days{7},
        std::nullopt,
        makeDefaultTimeFrame(),
        sprint_timer::api::TaskTypeDTO::Project};

    sprint_timer::api::TaskDTO project2{
        "p2",
        {},
        "project2",
        0,
        {},
        false,
        anchorTime - dw::Days{1} - std::chrono::hours{1},
        std::nullopt,
        makeDefaultTimeFrame(),
        sprint_timer::api::TaskTypeDTO::Project};

    sprint_timer::api::TaskDTO project3{
        "p3",
        {},
        "project3",
        0,
        {},
        false,
        anchorTime - dw::Days{30},
        std::nullopt,
        makeDefaultTimeFrame(),
        sprint_timer::api::TaskTypeDTO::Project};

    sprint_timer::api::TaskDTO folder3{"f3",
                                       {},
                                       "folder3",
                                       0,
                                       {},
                                       false,
                                       anchorTime - std::chrono::hours{1},
                                       std::nullopt,
                                       makeDefaultTimeFrame(),
                                       sprint_timer::api::TaskTypeDTO::Folder};

    sprint_timer::api::TaskDTO task1{
        "t1",
        {"Tag1"},
        "task1",
        4,
        {},
        false,
        anchorTime - dw::Days{15},
        sprint_timer::api::NoteDTO{"Some notes for task 1"},
        makeDefaultTimeFrame(),
        sprint_timer::api::TaskTypeDTO::Regular};

    sprint_timer::api::TaskDTO project4{
        "p4",
        {"Tag2"},
        "project4",
        0,
        {},
        false,
        anchorTime,
        sprint_timer::api::NoteDTO{"Some notes for project3 1"},
        makeDefaultTimeFrame(),
        sprint_timer::api::TaskTypeDTO::Project};

    sprint_timer::api::TaskDTO task2{
        "t2",
        {"Tag2"},
        "task2",
        3,
        {},
        false,
        anchorTime - dw::Days{5},
        sprint_timer::api::NoteDTO{"Some notes for task 2"},
        sprint_timer::api::TaskTimeframeDTO{
            dw::DateTime{anchorTime - dw::Years{2}},
            dw::DateTime{anchorTime - dw::Years{2} + dw::Weeks{3}},
            std::nullopt,
            std::nullopt},
        sprint_timer::api::TaskTypeDTO::Regular};

    sprint_timer::api::TaskDTO task3{
        "t3",
        {"Tag3"},
        "task3",
        4,
        {},
        false,
        anchorTime - dw::Days{2},
        sprint_timer::api::NoteDTO{"Some notes for task 3"},
        makeDefaultTimeFrame(),
        sprint_timer::api::TaskTypeDTO::Regular};

    sprint_timer::api::TaskDTO task4{"t4",
                                     {"Tag4"},
                                     "task4",
                                     4,
                                     {},
                                     false,
                                     anchorTime - std::chrono::minutes{3},
                                     std::nullopt,
                                     makeDefaultTimeFrame(),
                                     sprint_timer::api::TaskTypeDTO::Regular};

    // Note that this task has start time 20 days in the future
    sprint_timer::api::TaskDTO task5{
        "t5",
        {"Tag2, Tag5"},
        "task5",
        5,
        {},
        false,
        anchorTime - dw::Days{1},
        sprint_timer::api::NoteDTO{"Some notes for task 5"},
        sprint_timer::api::TaskTimeframeDTO{anchorTime + dw::Days{20},
                                            anchorTime + dw::Days{30},
                                            std::nullopt,
                                            std::nullopt},
        sprint_timer::api::TaskTypeDTO::Regular};

    sprint_timer::api::TaskDTO task6{
        "t6",
        {"Tag6"},
        "task6",
        5,
        {dw::DateTimeRange{anchorTime, anchorTime + 25min},
         dw::DateTimeRange{anchorTime, anchorTime + 25min},
         dw::DateTimeRange{anchorTime, anchorTime + 25min},
         dw::DateTimeRange{anchorTime, anchorTime + 25min},
         dw::DateTimeRange{anchorTime, anchorTime + 25min}},
        true,
        anchorTime - dw::Days{32},
        sprint_timer::api::NoteDTO{"Some notes for task 6"},
        makeDefaultTimeFrame(),
        sprint_timer::api::TaskTypeDTO::Regular};

    sprint_timer::api::TaskDTO recurringTask1{
        "r1",
        {"Tag1", "Tag3"},
        "recurringTask1",
        2,
        {},
        false,
        anchorTime - dw::Days{4},
        sprint_timer::api::NoteDTO{"Some notes for recurringTask1"},
        makeDefaultTimeFrame(),
        sprint_timer::api::TaskTypeDTO::Regular};

    sprint_timer::api::TaskDTO task7{
        "t7",
        {"Tag7"},
        "task7",
        4,
        {},
        false,
        anchorTime - dw::Days{6},
        sprint_timer::api::NoteDTO{"Some notes for task 7"},
        makeDefaultTimeFrame(),
        sprint_timer::api::TaskTypeDTO::Regular};

    sprint_timer::api::TaskTreeDTO tree;

    auto make_tree() -> void
    {
        /*
         * folder1
         *    folder2
         *       project1
         *          project2
         *          project3
         *             folder3
         *                task1
         * project4
         *    task2
         *       task3
         *       task4
         *          task5
         *          recurringTask1
         *    task6
         * task7
         */
        auto uuid_projection = [](const auto& node) { return node.uuid; };
        tree.insert(tree.end(), folder1);
        tree.insert(std::ranges::find(tree, folder1.uuid, uuid_projection),
                    folder2);
        tree.insert(std::ranges::find(tree, folder2.uuid, uuid_projection),
                    project1);
        tree.insert(std::ranges::find(tree, project1.uuid, uuid_projection),
                    project2);
        tree.insert(std::ranges::find(tree, project1.uuid, uuid_projection),
                    project3);
        tree.insert(std::ranges::find(tree, project3.uuid, uuid_projection),
                    folder3);
        tree.insert(std::ranges::find(tree, folder3.uuid, uuid_projection),
                    task1);
        tree.insert(tree.end(), project4);
        tree.insert(std::ranges::find(tree, project4.uuid, uuid_projection),
                    task2);
        tree.insert(std::ranges::find(tree, task2.uuid, uuid_projection),
                    task3);
        tree.insert(std::ranges::find(tree, task2.uuid, uuid_projection),
                    task4);
        tree.insert(std::ranges::find(tree, task4.uuid, uuid_projection),
                    task5);
        tree.insert(std::ranges::find(tree, task4.uuid, uuid_projection),
                    recurringTask1);
        tree.insert(std::ranges::find(tree, project4.uuid, uuid_projection),
                    task6);
        tree.insert(tree.end(), task7);
        // return tree;
    }
};

} // namespace fixtures

#endif /* end of include guard: TASKDTOTREEFIXTURES_H_ALQPXRZG */
