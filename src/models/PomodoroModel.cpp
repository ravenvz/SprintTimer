#include "models/PomodoroModel.h"
#include "core/use_cases/use_cases.h"

PomodoroModel::PomodoroModel(
    std::unique_ptr<IPomodoroStorageReader> pomodoroStorageReader,
    std::unique_ptr<IPomodoroStorageWriter> pomodoroStorageWriter,
    std::unique_ptr<ITaskStorageWriter> taskStorageWriter,
    QObject* parent)
    : QAbstractListModel(parent)
    , interval{TimeSpan{DateTime::currentDateTime(),
                        DateTime::currentDateTime()}}
    , pomodoroReader{std::move(pomodoroStorageReader)}
    , pomodoroWriter{std::move(pomodoroStorageWriter)}
    , taskWriter{std::move(taskStorageWriter)}
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
    CoreApi::addPomodoro(
        *pomodoroWriter, *taskWriter, pomodoro.timeSpan(), pomodoro.taskUuid());
    retrieveData();
}

void PomodoroModel::remove(int row)
{
    CoreApi::removePomodoro(
        *pomodoroWriter, *taskWriter, storage[static_cast<size_t>(row)]);
    retrieveData();
}

void PomodoroModel::retrieveData()
{
    CoreApi::pomodorosInTimeRange(
        *pomodoroReader,
        interval,
        std::bind(&PomodoroModel::onDataChanged, this, std::placeholders::_1));
}

void PomodoroModel::onDataChanged(const std::vector<Pomodoro>& items)
{
    beginResetModel();
    storage = items;
    endResetModel();
}
