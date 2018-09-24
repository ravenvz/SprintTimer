#ifndef CONNECTIONGUARD_H_6GDKXRYB
#define CONNECTIONGUARD_H_6GDKXRYB

#include <QString>

namespace sprint_timer::storage::qt_storage_impl {

struct ConnectionGuard {
    ConnectionGuard(const QString& dbName, const QString& connectionName);

    ~ConnectionGuard();

    ConnectionGuard(ConnectionGuard&&) = delete;
    ConnectionGuard& operator=(ConnectionGuard&&) = delete;

    ConnectionGuard(const ConnectionGuard&) = delete;
    ConnectionGuard& operator=(const ConnectionGuard&) = delete;

private:
    const QString connectionName;
};


} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: CONNECTIONGUARD_H_6GDKXRYB */
