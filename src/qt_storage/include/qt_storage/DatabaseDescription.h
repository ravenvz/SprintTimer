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
#ifndef DATABASEDESCRIPTION_H_035DMAOX
#define DATABASEDESCRIPTION_H_035DMAOX

#include <QString>
#include <QStringView>
#include <string_view>

namespace sprint_timer::storage::qt_storage {

namespace InfoTable {
    inline const QStringView name{QStringLiteral("info")};

    namespace Columns {
        inline const QStringView id{QStringLiteral("id")};
        inline const QStringView name{QStringLiteral("name")};
        inline const QStringView value{QStringLiteral("value")};
    } // namespace Columns

} // namespace InfoTable

namespace TaskTable {
    const QStringView name{QStringLiteral("task")};

    namespace Columns {
        inline const QStringView id{QStringLiteral("id")};
        inline const QStringView name{QStringLiteral("name")};
        inline const QStringView estimatedCost{
            QStringLiteral("estimated_cost")};
        inline const QStringView actualCost{QStringLiteral("actual_cost")};
        inline const QStringView completed{QStringLiteral("completed")};
        inline const QStringView priority{QStringLiteral("priority")};
        inline const QStringView lastModified{QStringLiteral("last_modified")};
        inline const QStringView uuid{QStringLiteral("uuid")};
    } // namespace Columns

} // namespace TaskTable

namespace SprintTable {
    inline const QStringView name{QStringLiteral("sprint")};

    namespace Columns {
        inline const QStringView id{QStringLiteral("id")};
        inline const QStringView taskUuid{QStringLiteral("task_uuid")};
        inline const QStringView startTime{QStringLiteral("start_time")};
        inline const QStringView finishTime{QStringLiteral("finish_time")};
        inline const QStringView uuid{QStringLiteral("uuid")};
    } // namespace Columns

} // namespace SprintTable

namespace TagTable {
    inline const QStringView name{QStringLiteral("tag")};

    namespace Columns {
        inline const QStringView id{QStringLiteral("id")};
        inline const QStringView name{QStringLiteral("name")};
    } // namespace Columns
} // namespace TagTable

namespace TaskTagTable {
    inline const QStringView name{QStringLiteral("tasktag")};

    namespace Columns {
        inline const QStringView id{QStringLiteral("id")};
        inline const QStringView tagId{QStringLiteral("tag_id")};
        inline const QStringView taskId{QStringLiteral("task_id")};
        inline const QStringView taskUuid{QStringLiteral("task_uuid")};
    } // namespace Columns

} // namespace TaskTagTable

namespace ExceptionalDayTable {
    inline const QStringView name{QStringLiteral("exceptional_day")};

    namespace Columns {
        inline const QStringView id{QStringLiteral("id")};
        inline const QStringView date{QStringLiteral("date")};
        inline const QStringView goal{QStringLiteral("goal")};
    } // namespace Columns

} // namespace ExceptionalDayTable

namespace ScheduleTable {
    inline const QStringView name{QStringLiteral("schedule")};

    namespace Columns {
        inline const QStringView id{QStringLiteral("id")};
        inline const QStringView applied_since{QStringLiteral("applied_since")};
        inline const QStringView monday_goal{QStringLiteral("mon_goal")};
        inline const QStringView tuesday_goal{QStringLiteral("tue_goal")};
        inline const QStringView wednesday_goal{QStringLiteral("wed_goal")};
        inline const QStringView thursday_goal{QStringLiteral("thu_goal")};
        inline const QStringView friday_goal{QStringLiteral("fri_goal")};
        inline const QStringView saturday_goal{QStringLiteral("sat_goal")};
        inline const QStringView sunday_goal{QStringLiteral("sun_goal")};
    } // namespace Columns

} // namespace ScheduleTable

namespace TaskTagView {
    inline const QStringView name{QStringLiteral("task_tag_view")};

    namespace Aliases {
        inline const QStringView tagName{QStringLiteral("tagname")};
    } // namespace Aliases

} // namespace TaskTagView

namespace TaskTagInsertTrigger {
    inline const QStringView name{
        QStringLiteral("instead_task_tag_view_insert")};
} // namespace TaskTagInsertTrigger

namespace CleanOrphanedTagTrigger {
    inline const QStringView name{QStringLiteral("clean_orhaned_tag")};
} // namespace CleanOrphanedTagTrigger

namespace SprintView {
    inline const QStringView name{QStringLiteral("sprint_view")};

    namespace Aliases {
        inline const QStringView tags{QStringLiteral("tags")};
    }

} // namespace SprintView

namespace SprintViewDeleteTrigger {
    inline const QStringView name{QStringLiteral("delete_from_sprint_view")};
} // namespace SprintViewDeleteTrigger

namespace SprintViewInsertTrigger {
    inline const QStringView name{QStringLiteral("instead_sprint_view_insert")};
} // namespace SprintViewInsertTrigger

namespace TasksView {
    inline const QStringView name{QStringLiteral("task_view")};

    namespace Aliases {
        inline const QStringView tags{QStringLiteral("tags")};
    } // namespace Aliases

} // namespace TasksView

namespace TaskViewDeleteTrigger {
    inline const QStringView name{QStringLiteral("on_task_view_delete")};
} // namespace TaskViewDeleteTrigger

namespace TaskViewUpdateTrigger {
    inline const QStringView name{QStringLiteral("on_task_view_update")};
} // namespace TaskViewUpdateTrigger

namespace CalendarTable {
    inline const QStringView name{QStringLiteral("calendar")};

    namespace Columns {
        inline const QStringView id{QStringLiteral("id")};
        inline const QStringView dt{QStringLiteral("dt")};
    } // namespace Columns

} // namespace CalendarTable

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: DATABASEDESCRIPTION_H_035DMAOX */
