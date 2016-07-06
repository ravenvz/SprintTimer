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
#ifndef FAKEPOMODOROWRITER_H_IHZ1QWFR
#define FAKEPOMODOROWRITER_H_IHZ1QWFR

#include "core/IPomodoroStorageWriter.h"
#include "FakeStorage.h"

/* Fake implementation of IPomodoroStorageWriter that exposes
 * internal storage and allows to test execute/undo. */
class FakePomodoroWriter : public IPomodoroStorageWriter {
public:
    FakePomodoroWriter(FakeStorage<Pomodoro>& storage)
        : storage{storage}
    {
    }

    void save(const Pomodoro& pomodoro) final { storage.store(pomodoro); }

    void remove(const Pomodoro& pomodoro) final
    {
        storage.remove(pomodoro.uuid());
    }

    FakeStorage<Pomodoro>& storage;
};


#endif /* end of include guard: FAKEPOMODOROWRITER_H_IHZ1QWFR */
