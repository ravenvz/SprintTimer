#ifndef QTTASKSTORAGEWRITER_H_AB4O73ZJ
#define QTTASKSTORAGEWRITER_H_AB4O73ZJ

#include "core/ITaskStorageWriter.h"
#include "qt_storage_impl/db_service.h"
#include <QObject>
#include <algorithm>

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
    long long addTaskQueryId{-1};
    long long insertTagQueryId{-1};
    long long removeTagQueryId{-1};
    long long removeTaskQueryId{-1};
    long long editQueryId{-1};
    long long incrementSpentQueryId{-1};
    long long decrementSpentQueryId{-1};

    void insertTags(const QString& taskUuid,
                    const std::list<std::string>& tags);

    void removeTags(const QString& taskUuid,
                    const std::list<std::string>& tags);
};

template <class InputIt1, class InputIt2, class OutputIt1, class OutputIt2>
void twoWayDiff(InputIt1 first1,
                InputIt1 last1,
                InputIt2 first2,
                InputIt2 last2,
                OutputIt1 out1,
                OutputIt2 out2)
{
    std::set_difference(first1, last1, first2, last2, out1);
    std::set_difference(first2, last2, first1, last1, out2);
}

#endif /* end of include guard: QTTASKSTORAGEWRITER_H_AB4O73ZJ */
