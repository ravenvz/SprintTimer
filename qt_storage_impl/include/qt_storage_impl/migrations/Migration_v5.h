#ifndef MIGRATION_V5_H_YR9JKJTX
#define MIGRATION_V5_H_YR9JKJTX

#include "qt_storage_impl/migrations/Migration.h"

namespace sprint_timer::storage::qt_storage_impl {

class Migration_v5 final : public Migration {
public:
    void run(QSqlDatabase& database) const final;
};


} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: MIGRATION_V5_H_YR9JKJTX */
