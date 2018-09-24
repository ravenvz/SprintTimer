#ifndef MIGRATION_V3_H_DCMNQEBQ
#define MIGRATION_V3_H_DCMNQEBQ

#include "qt_storage_impl/migrations/Migration.h"

namespace sprint_timer::storage::qt_storage_impl {

class Migration_v3 final : public Migration {
public:
    void run(QSqlDatabase& database) const final;
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: MIGRATION_V3_H_DCMNQEBQ */
