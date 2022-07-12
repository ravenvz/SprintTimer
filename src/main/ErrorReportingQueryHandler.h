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
#ifndef ERRORREPORTINGQUERYHANDLER_H_PSWFEEGV
#define ERRORREPORTINGQUERYHANDLER_H_PSWFEEGV

#include "api/com_query/Query.h"
#include "api/com_query/RequestHandlerDecorator.h"
#include "core/SprintConflictException.h"
#include "core/SprintTimerException.h"
#include "qt_storage/QueryError.h"
#include <QMessageBox>
#include <sstream>
#include <string_view>

namespace sprint_timer {

template <asp::Query QueryT>
class ErrorReportingQueryHandler : public asp::RequestHandlerDecorator<QueryT> {
public:
    explicit ErrorReportingQueryHandler(
        std::unique_ptr<asp::RequestHandler<QueryT>> wrapped);

    QueryT::Result handle(const QueryT& command) override;
};

template <asp::Query QueryT>
ErrorReportingQueryHandler<QueryT>::ErrorReportingQueryHandler(
    std::unique_ptr<asp::RequestHandler<QueryT>> wrapped_)
    : asp::RequestHandlerDecorator<QueryT>{std::move(wrapped_)}
{
}

template <asp::Query QueryT>
QueryT::Result ErrorReportingQueryHandler<QueryT>::handle(const QueryT& request)
{
    try {
        return asp::RequestHandlerDecorator<QueryT>::handle(request);
    }
    catch (storage::qt_storage::QueryError& exc) {
        std::stringstream ss;
        ss << exc.queryText() << '\n';
        ss << exc.queryError() << '\n';
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(ss.str()));
        msgBox.exec();
        throw;
    }
    catch (SprintTimerException& exc) {
        QMessageBox msgBox;
        msgBox.setText("Exception when handling command");
        msgBox.setInformativeText(exc.what());
        msgBox.exec();
        throw;
    }
    catch (std::exception& exc) {
        QMessageBox msgBox;
        std::stringstream ss;
        msgBox.setText("Exception when handling request");
        ss << request << " \n with description: " << exc.what();
        msgBox.setText(QString::fromStdString(ss.str()));
        msgBox.exec();
        throw;
    }
}

} // namespace sprint_timer

#endif /* end of include guard: ERRORREPORTINGQUERYHANDLER_H_PSWFEEGV */
