/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#ifndef QTSPRINTSTORAGEWRITER_H_U7AAXVTC
#define QTSPRINTSTORAGEWRITER_H_U7AAXVTC

#include "core/ISprintStorageWriter.h"
#include "qt_storage_impl/DBService.h"
#include <QObject>

class QtSprintStorageWriter : public QObject, public ISprintStorageWriter {
    Q_OBJECT

public:
    explicit QtSprintStorageWriter(DBService& dbService);

    void save(const Sprint& sprint) final;

    void remove(const Sprint& sprint) final;

private:
    DBService& dbService;
    long long addQueryId{-1};
    long long removeQueryId{-1};
};

#endif /* end of include guard: QTSPRINTSTORAGEWRITER_H_U7AAXVTC */
