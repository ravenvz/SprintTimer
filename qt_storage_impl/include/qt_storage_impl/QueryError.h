#ifndef QUERYERROR_H_WQUWETDP
#define QUERYERROR_H_WQUWETDP

#include <QSqlQuery>
#include <stdexcept>

namespace sprint_timer::storage::qt_storage_impl {

class QueryError : public std::runtime_error {
public:
    QueryError(const std::string& description, const QSqlQuery& query);

    std::string queryText() const;

    std::string queryError() const;

private:
    const std::string query;
    const std::string lastError;
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: QUERYERROR_H_WQUWETDP */
