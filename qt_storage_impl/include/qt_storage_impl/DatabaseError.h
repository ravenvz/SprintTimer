#ifndef DATABASEERROR_H_CRP0CBTN
#define DATABASEERROR_H_CRP0CBTN

#include <QSqlDatabase>
#include <stdexcept>
#include <string>

namespace sprint_timer::storage::qt_storage_impl {

class DatabaseError : public std::runtime_error {
public:
    DatabaseError(const std::string& description, const QSqlDatabase& database);

    std::string error() const;

private:
    const std::string error_;
};


} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: DATABASEERROR_H_CRP0CBTN */
