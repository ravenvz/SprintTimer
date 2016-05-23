#ifndef QTTASKSTORAGEWRITER_H_AB4O73ZJ
#define QTTASKSTORAGEWRITER_H_AB4O73ZJ

#include "core/ITaskStorageWriter.h"
#include "db_layer/db_service.h"
#include <QObject>

class QtTaskStorageWriter : public QObject, public ITaskStorageWriter {
    Q_OBJECT

public:
    explicit QtTaskStorageWriter(DBService& dbService);

    void save(const TodoItem& task) final;

    void remove(const TodoItem& task) final;

    void edit(const TodoItem& task, const TodoItem& editedTask) final;

    void incrementSpentPomodoros(const std::string& uuid) final;

    void decrementSpentPomodoros(const std::string& uuid) final;

private:
    DBService& dbService;
    long long addQueryId;
    long long removeQueryId;
    long long editQueryId;
    long long incrementSpentQueryId;
    long long decrementSpentQueryId;
};

#endif /* end of include guard: QTTASKSTORAGEWRITER_H_AB4O73ZJ */
