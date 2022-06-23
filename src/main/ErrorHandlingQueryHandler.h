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
#ifndef ERRORHANDLINGQUERYHANDLER_H_SXNYKSCT
#define ERRORHANDLINGQUERYHANDLER_H_SXNYKSCT

#include "core/QueryHandler.h"
#include "qt_storage/QueryError.h"
#include <QMessageBox>
#include <iostream>
#include <memory>

namespace sprint_timer {

template <typename QueryT>
class ErrorHandlingQueryHandler : public QueryHandler<QueryT> {
public:
    ErrorHandlingQueryHandler(std::unique_ptr<QueryHandler<QueryT>> wrapped,
                              std::ostream& os);

    typename QueryT::result_t handle(QueryT&& query) override;

private:
    std::unique_ptr<QueryHandler<QueryT>> wrapped;
    std::ostream& os;
};

template <typename QueryT>
ErrorHandlingQueryHandler<QueryT>::ErrorHandlingQueryHandler(
    std::unique_ptr<QueryHandler<QueryT>> wrapped_, std::ostream& os_)
    : wrapped{std::move(wrapped_)}
    , os{os_}
{
}

template <typename QueryT>
typename QueryT::result_t
ErrorHandlingQueryHandler<QueryT>::handle(QueryT&& query)
{
    try {
        return wrapped->handle(std::move(query));
    }
    catch (storage::qt_storage::QueryError& exc) {
        os << exc.queryText() << '\n';
        os << exc.queryError() << '\n';
        throw;
    }
    catch (std::exception& exc) {
        QMessageBox msgBox;
        std::stringstream ss;
        msgBox.setText("Exception when handling query");
        ss << query << " \n with description: " << exc.what();
        msgBox.setText(QString::fromStdString(ss.str()));
        msgBox.exec();
        throw;
    }
}

} // namespace sprint_timer

#endif /* end of include guard: ERRORHANDLINGQUERYHANDLER_H_SXNYKSCT */
