#include "models/PomodoroModelNew.h"
#include "qt_storage_impl/QtSqlitePomodoroStorageReader.h"
#include "qt_storage_impl/QtSqlitePomodoroStorageWriter.h"
#include "core/use_cases/AddPomodoroTransaction.h"
#include "core/use_cases/RemovePomodoroTransaction.h"
#include "core/use_cases/RequestPomodorosInTimeRangeCommand.h"

PomodoroModelNew::PomodoroModelNew(DBService& dbService, QObject* parent)
    : QAbstractListModel(parent)
    , interval{TimeSpan{DateTime::currentDateTime(),
                        DateTime::currentDateTime()}}
    , reader{std::make_unique<QtSqlitePomodoroStorageReader>(dbService)}
    , writer{std::make_unique<QtSqlitePomodoroStorageWriter>(dbService)}
{
    retrieveData();
}

int PomodoroModelNew::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(storage.size());
}

QVariant PomodoroModelNew::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Pomodoro pomodoro = storage[static_cast<size_t>(index.row())];

    switch (role) {
    case Qt::DisplayRole:
        return QString::fromStdString(pomodoro.toString());
    }

    return QVariant();
}

void PomodoroModelNew::setDateFilter(const TimeSpan& timeSpan)
{
    interval = timeSpan;
    retrieveData();
}

void PomodoroModelNew::insert(const long long associatedTaskId,
                              const TimeSpan& timeSpan)
{
    Pomodoro pomodoro{timeSpan};
    UseCases::AddPomodoroTransaction addPomodoro{
        *writer, pomodoro, associatedTaskId};
    addPomodoro.execute();
    retrieveData();
}

// void PomodoroModelNew::remove(long long pomodoroId)
// {
//     // UseCases::RemovePomodoroTransaction removePomodoro{
//     //     *writer,
// }

void PomodoroModelNew::remove(int row)
{
    UseCases::RemovePomodoroTransaction removePomodoro{
        *writer, storage[static_cast<size_t>(row)]};
    removePomodoro.execute();
}

void PomodoroModelNew::retrieveData()
{
    UseCases::RequestPomodorosInTimeRangeCommand command{
        *reader,
        interval,
        std::bind(
            &PomodoroModelNew::onDataChanged, this, std::placeholders::_1)};
    command.execute();
}

void PomodoroModelNew::onDataChanged(const std::vector<Pomodoro>& items)
{
    beginResetModel();
    storage = items;
    endResetModel();
}
