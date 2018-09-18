#ifndef MIGRATIONMANAGER_H_PRWBSWIP
#define MIGRATIONMANAGER_H_PRWBSWIP

#include "qt_storage_impl/migrations/Migration.h"
#include <memory>
#include <unordered_map>

namespace sprint_timer::storage::qt_storage_impl {

class MigrationManager {
public:
    MigrationManager(unsigned currentDbVersion);

    void runMigrations(QSqlDatabase& database) const;

    void addMigration(unsigned version, std::unique_ptr<Migration> migration);

private:
    unsigned currentDatabaseVersion;
    std::unordered_map<unsigned, std::unique_ptr<Migration>> migrations;
};


} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: MIGRATIONMANAGER_H_PRWBSWIP */
