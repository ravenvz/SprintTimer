#ifndef QTTASKSTORAGEREADER_H_48BDKQZ7
#define QTTASKSTORAGEREADER_H_48BDKQZ7

#include "core/ITaskStorageReader.h"
#include "qt_storage_impl/db_service.h"
#include <QObject>

class QtTaskStorageReader : public QObject, public ITaskStorageReader {
    Q_OBJECT

public:
    explicit QtTaskStorageReader(DBService& dbService);

    void requestUnfinishedTasks(Handler handler) final;

    void requestFinishedTasks(const TimeSpan& timeSpan, Handler handler) final;

    void requestAllTags(TagHandler handler) final;

private:
    enum class Column {
        Id,
        Name,
        EstimatedPomodoros,
        SpentPomodoros,
        Priority,
        Completed,
        Tags,
        LastModified,
        Uuid
    };

    enum class TagColumn { Id, Name };

    long long mUnfinishedQueryId{-1};
    long long mFinishedQueryId{-1};
    long long mTagQueryId{-1};
    DBService& dbService;
    std::list<Handler> handler_queue;
    std::list<TagHandler> tag_handler_queue;

    QVariant columnData(const QSqlRecord& record, Column column) const;

    TodoItem taskFromQSqlRecord(const QSqlRecord& record) const;

    std::string tagFromSqlRecord(const QSqlRecord& record) const;

private slots:
    void onResultsReceived(long long queryId,
                           const std::vector<QSqlRecord>& records);
};

#endif /* end of include guard: QTTASKSTORAGEREADER_H_48BDKQZ7 */
