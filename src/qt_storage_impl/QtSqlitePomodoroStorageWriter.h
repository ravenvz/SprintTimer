#ifndef QTSQLITEPOMODOROSTORAGEWRITER_H_U7AAXVTC
#define QTSQLITEPOMODOROSTORAGEWRITER_H_U7AAXVTC

#include <QObject>
#include "core/IPomodoroStorageWriter.h"
#include "db_layer/db_service.h"

class QtSqlitePomodoroStorageWriter : public QObject,
                                      public IPomodoroStorageWriter {
    Q_OBJECT

public:
    explicit QtSqlitePomodoroStorageWriter(DBService& dbService);

    void save(const Pomodoro& pomodoro, long long taskId) final;

    void remove(const Pomodoro& pomodoro) final;

private:
    DBService& dbService;
    QString addQueryId{"AddPomodoro"};
    QString removeQueryId{"RemovePomodoro"};

private slots:
    void onError(const QString& errorMessage);
};

#endif /* end of include guard: QTSQLITEPOMODOROSTORAGEWRITER_H_U7AAXVTC */
