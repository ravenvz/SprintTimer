#ifndef MIGRATION_H_BHJNGVB8
#define MIGRATION_H_BHJNGVB8

#include <QSqlDatabase>

namespace sprint_timer::storage::qt_storage_impl {


class Migration {
public:
    virtual ~Migration() = default;

    virtual void run(QSqlDatabase& database) const = 0;
};


} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: MIGRATION_H_BHJNGVB8 */
