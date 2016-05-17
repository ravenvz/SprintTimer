#ifndef QTSQLITEPOMODOROSTORAGEWRITER_H_U7AAXVTC
#define QTSQLITEPOMODOROSTORAGEWRITER_H_U7AAXVTC

#include "core/IPomodoroStorageWriter.h"
#include "db_layer/db_service.h"
#include <QObject>

class QtPomoStorageWriter : public QObject,
                                      public IPomodoroStorageWriter {
    Q_OBJECT

public:
    explicit QtPomoStorageWriter(DBService& dbService);

    void save(const Pomodoro& pomodoro) final;

    void remove(const Pomodoro& pomodoro) final;

private:
    DBService& dbService;
    QString addQueryId{"AddPomodoro"};
    QString removeQueryId{"RemovePomodoro"};
};

#endif /* end of include guard: QTSQLITEPOMODOROSTORAGEWRITER_H_U7AAXVTC */
