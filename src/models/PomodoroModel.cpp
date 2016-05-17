#include "models/PomodoroModel.h"
#include "core/use_cases/AddPomodoroTransaction.h"
#include "core/use_cases/RemovePomodoroTransaction.h"
#include "core/use_cases/RequestPomodorosInTimeRangeCommand.h"
#include "qt_storage_impl/QtPomoStorageReader.h"
#include "qt_storage_impl/QtPomoStorageWriter.h"

PomodoroModel::PomodoroModel(DBService& dbService, QObject* parent)
    : QAbstractListModel(parent)
    , interval{TimeSpan{DateTime::currentDateTime(),
                        DateTime::currentDateTime()}}
    , reader{std::make_unique<QtPomoStorageReader>(dbService)}
    , writer{std::make_unique<QtPomoStorageWriter>(dbService)}
{
    retrieveData();
}

int PomodoroModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(storage.size());
}

QVariant PomodoroModel::data(const QModelIndex& index, int role) const
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

void PomodoroModel::setDateFilter(const TimeSpan& timeSpan)
{
    interval = timeSpan;
    retrieveData();
}

void PomodoroModel::insert(const Pomodoro& pomodoro)
{
    UseCases::AddPomodoroTransaction addPomodoro{*writer, pomodoro};
    addPomodoro.execute();
    retrieveData();
}

void PomodoroModel::remove(int row)
{
    UseCases::RemovePomodoroTransaction removePomodoro{
        *writer, storage[static_cast<size_t>(row)]};
    removePomodoro.execute();
    retrieveData();
}

void PomodoroModel::retrieveData()
{
    UseCases::RequestPomodorosInTimeRangeCommand command{
        *reader,
        interval,
        std::bind(
            &PomodoroModel::onDataChanged, this, std::placeholders::_1)};
    command.execute();
}

void PomodoroModel::onDataChanged(const std::vector<Pomodoro>& items)
{
    beginResetModel();
    storage = items;
    endResetModel();
}
