#include "models/PomodoroModelNew.h"
#include "core/use_cases/AddPomodoroTransaction.h"
#include "core/use_cases/RemovePomodoroTransaction.h"
#include "core/use_cases/RequestPomodorosInTimeRangeCommand.h"
#include "qt_storage_impl/QtPomoStorageReader.h"
#include "qt_storage_impl/QtPomoStorageWriter.h"

PomodoroModelNew::PomodoroModelNew(DBService& dbService, QObject* parent)
    : QAbstractListModel(parent)
    , interval{TimeSpan{DateTime::currentDateTime(),
                        DateTime::currentDateTime()}}
    , reader{std::make_unique<QtPomoStorageReader>(dbService)}
    , writer{std::make_unique<QtPomoStorageWriter>(dbService)}
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

void PomodoroModelNew::insert(const Pomodoro& pomodoro)
{
    UseCases::AddPomodoroTransaction addPomodoro{*writer, pomodoro};
    addPomodoro.execute();
    retrieveData();
}

void PomodoroModelNew::remove(int row)
{
    UseCases::RemovePomodoroTransaction removePomodoro{
        *writer, storage[static_cast<size_t>(row)]};
    removePomodoro.execute();
    retrieveData();
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
