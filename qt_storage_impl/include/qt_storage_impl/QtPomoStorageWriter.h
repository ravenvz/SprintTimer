/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef QTSQLITEPOMODOROSTORAGEWRITER_H_U7AAXVTC
#define QTSQLITEPOMODOROSTORAGEWRITER_H_U7AAXVTC

#include "core/IPomodoroStorageWriter.h"
#include "qt_storage_impl/db_service.h"
#include <QObject>

class QtPomoStorageWriter : public QObject, public IPomodoroStorageWriter {
    Q_OBJECT

public:
    explicit QtPomoStorageWriter(DBService& dbService);

    void save(const Pomodoro& pomodoro) final;

    void remove(const Pomodoro& pomodoro) final;

private:
    DBService& dbService;
    long long addQueryId{-1};
    long long removeQueryId{-1};
};

#endif /* end of include guard: QTSQLITEPOMODOROSTORAGEWRITER_H_U7AAXVTC */
