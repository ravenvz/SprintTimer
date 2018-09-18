#ifndef DATABASEDESCRIPTION_H_035DMAOX
#define DATABASEDESCRIPTION_H_035DMAOX

#include <QString>

namespace sprint_timer::storage::qt_storage_impl {

namespace InfoTable {
    inline const QString name{QStringLiteral("info")};

    namespace Columns {
        inline const QString id{QStringLiteral("id")};
        inline const QString name{QStringLiteral("name")};
        inline const QString value{QStringLiteral("value")};
    } // namespace Columns

} // namespace InfoTable

namespace TaskTable {
    const QString name{"task"};

    namespace Columns {
        inline const QString id{QStringLiteral("id")};
        inline const QString name{QStringLiteral("name")};
        inline const QString estimatedCost{QStringLiteral("estimated_cost")};
        inline const QString actualCost{QStringLiteral("actual_cost")};
        inline const QString completed{QStringLiteral("completed")};
        inline const QString priority{QStringLiteral("priority")};
        inline const QString lastModified{QStringLiteral("last_modified")};
        inline const QString uuid{QStringLiteral("uuid")};
    } // namespace Columns

} // namespace TaskTable

namespace SprintTable {
    inline const QString name{QStringLiteral("sprint")};

    namespace Columns {
        inline const QString id{QStringLiteral("id")};
        inline const QString taskUuid{QStringLiteral("task_uuid")};
        inline const QString startTime{QStringLiteral("start_time")};
        inline const QString finishTime{QStringLiteral("finish_time")};
        inline const QString uuid{QStringLiteral("uuid")};
    } // namespace Columns

} // namespace SprintTable

namespace TagTable {
    inline const QString name{QStringLiteral("tag")};

    namespace Columns {
        inline const QString id{QStringLiteral("id")};
        inline const QString name{QStringLiteral("name")};
    } // namespace Columns
} // namespace TagTable

namespace TaskTagTable {
    inline const QString name{QStringLiteral("tasktag")};

    namespace Columns {
        inline const QString id{QStringLiteral("id")};
        inline const QString tagId{QStringLiteral("tag_id")};
        inline const QString taskId{QStringLiteral("task_id")};
        inline const QString taskUuid{QStringLiteral("task_uuid")};
    } // namespace Columns

} // namespace TaskTagTable

namespace TaskTagView {
    inline const QString name{QStringLiteral("task_tag_view")};

    namespace Aliases {
        inline const QString tagName{QStringLiteral("tagname")};
    } // namespace Aliases

} // namespace TaskTagView

namespace TaskTagInsertTrigger {
    inline const QString name{QStringLiteral("instead_task_tag_view_insert")};
} // namespace TaskTagInsertTrigger

namespace CleanOrphanedTagTrigger {
    inline const QString name{QStringLiteral("clean_orhaned_tag")};
} // namespace CleanOrphanedTagTrigger

namespace SprintView {
    inline const QString name{QStringLiteral("sprint_view")};

    namespace Aliases {
        inline const QString tags{QStringLiteral("tags")};
    }

} // namespace SprintView

namespace SprintViewDeleteTrigger {
    inline const QString name{QStringLiteral("delete_from_sprint_view")};
} // namespace SprintViewDeleteTrigger

namespace SprintViewInsertTrigger {
    inline const QString name{QStringLiteral("instead_sprint_view_insert")};
} // namespace SprintViewInsertTrigger

namespace TasksView {
    inline const QString name{QStringLiteral("task_view")};

    namespace Aliases {
        inline const QString tags{QStringLiteral("tags")};
    } // namespace Aliases

} // namespace TasksView

namespace TaskViewDeleteTrigger {
    inline const QString name{QStringLiteral("on_task_view_delete")};
} // namespace TaskViewDeleteTrigger

namespace TaskViewUpdateTrigger {
    inline const QString name{QStringLiteral("on_task_view_update")};
} // namespace TaskViewUpdateTrigger

namespace CalendarTable {
    inline const QString name{QStringLiteral("calendar")};

    namespace Columns {
        inline const QString id{QStringLiteral("id")};
        inline const QString dt{QStringLiteral("dt")};
    } // namespace Columns

} // namespace CalendarTable

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: DATABASEDESCRIPTION_H_035DMAOX */
