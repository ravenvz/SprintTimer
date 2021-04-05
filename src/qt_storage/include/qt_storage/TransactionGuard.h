/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#ifndef TRANSACTIONGUARD_H_R0K3BHPX
#define TRANSACTIONGUARD_H_R0K3BHPX

#include <QString>

namespace sprint_timer::storage::qt_storage {

class TransactionGuard {
public:
    TransactionGuard(const QString& connectionName);

    ~TransactionGuard();

    TransactionGuard(TransactionGuard&&) = delete;
    TransactionGuard& operator=(TransactionGuard&&) = delete;

    TransactionGuard(const TransactionGuard&) = delete;
    TransactionGuard& operator=(const TransactionGuard&) = delete;

    void commit();

private:
    QString connectionName;
    bool hasCommited{false};
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: TRANSACTIONGUARD_H_R0K3BHPX */

