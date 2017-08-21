/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#ifndef QTTASKSTORAGEWRITER_H_AB4O73ZJ
#define QTTASKSTORAGEWRITER_H_AB4O73ZJ

#include "core/ITaskStorageWriter.h"
#include "core/entities/Tag.h"
#include "qt_storage_impl/DBService.h"
#include <QObject>

class QtTaskStorageWriter : public QObject, public ITaskStorageWriter {
    Q_OBJECT

public:
    explicit QtTaskStorageWriter(DBService& dbService);

    void save(const Task& task) final;

    void remove(const Task& task) final;

    void edit(const Task& task, const Task& editedTask) final;

    void incrementSprints(const std::string& uuid) final;

    void decrementSprints(const std::string& uuid) final;

    void toggleTaskCompletionStatus(const std::string& uuid,
                                    const dw::DateTime& timeStamp) final;

    void updatePriorities(const std::vector<std::string>& priorities) final;

    void editTag(const std::string& oldName, const std::string& newName) final;

private:
    DBService& dbService;
    qint64 addTaskQueryId{-1};
    qint64 insertTagQueryId{-1};
    qint64 removeTagQueryId{-1};
    qint64 removeTaskQueryId{-1};
    qint64 editQueryId{-1};
    qint64 incrementSpentQueryId{-1};
    qint64 decrementSpentQueryId{-1};
    qint64 toggleCompletionQueryId{-1};
    qint64 updatePrioritiesQueryId{-1};
    qint64 editTagQueryId{-1};

    void insertTags(const QString& taskUuid, const std::list<Tag>& tagNames);

    void removeTags(const QString& taskUuid, const std::list<Tag>& tags);
};

#endif /* end of include guard: QTTASKSTORAGEWRITER_H_AB4O73ZJ */
