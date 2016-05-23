#ifndef POMODOROMODELNEW_H_MQZ2XAPI
#define POMODOROMODELNEW_H_MQZ2XAPI

#include "core/IPomodoroStorageReader.h"
#include "core/IPomodoroStorageWriter.h"
#include "core/ITaskStorageWriter.h"
#include "core/TimeSpan.h"
#include "core/entities/Pomodoro.h"
#include <QAbstractListModel>
#include <memory>
#include <vector>

class PomodoroModel : public QAbstractListModel {
public:
    explicit PomodoroModel(
        std::unique_ptr<IPomodoroStorageReader> pomodoroStorageReader,
        std::unique_ptr<IPomodoroStorageWriter> pomodoroStorageWriter,
        std::unique_ptr<ITaskStorageWriter> taskStorageWriter,
        QObject* parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const final;

    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const final;

    void setDateFilter(const TimeSpan& timeSpan);

    void insert(const Pomodoro& pomodoro);

    // void remove(long long pomodoroId);
    void remove(int row);

private:
    std::vector<Pomodoro> storage;
    TimeSpan interval;
    std::unique_ptr<IPomodoroStorageReader> pomodoroReader;
    std::unique_ptr<IPomodoroStorageWriter> pomodoroWriter;
    std::unique_ptr<ITaskStorageWriter> taskWriter;

    void retrieveData();

    void onDataChanged(const std::vector<Pomodoro>& items);
};

#endif /* end of include guard: POMODOROMODELNEW_H_MQZ2XAPI */
