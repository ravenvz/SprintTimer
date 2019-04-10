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
#ifndef FAKESPRINTWRITER_H_IHZ1QWFR
#define FAKESPRINTWRITER_H_IHZ1QWFR

#include "core/ISprintStorageWriter.h"
#include "FakeStorage.h"

/* Fake implementation of ISprintStorageWriter that exposes
 * internal storage and allows to test execute/undo. */
class FakeSprintWriter : public ISprintStorageWriter {
public:
    FakeSprintWriter(FakeStorage<Sprint>& storage)
        : storage{storage}
    {
    }

    void save(const Sprint& sprint) final { storage.store(sprint); }

    void remove(const Sprint& sprint) final
    {
        storage.remove(sprint.uuid());
    }

    FakeStorage<Sprint>& storage;
};


#endif /* end of include guard: FAKESPRINTWRITER_H_IHZ1QWFR */
