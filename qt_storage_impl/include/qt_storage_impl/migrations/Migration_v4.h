#ifndef MIGRATION_V4_H_EWUAYZMH
#define MIGRATION_V4_H_EWUAYZMH

#include "qt_storage_impl/migrations/Migration.h"

namespace sprint_timer::storage::qt_storage_impl {

class Migration_v4 final : public Migration {
public:
    void run(QSqlDatabase& database) const final;
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: MIGRATION_V4_H_EWUAYZMH */
