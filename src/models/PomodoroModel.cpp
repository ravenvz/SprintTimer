#include "models/PomodoroModel.h"

PomodoroModel::PomodoroModel(CoreApi::PomodoroService& pomodoroService,
                             QObject* parent)
    : QAbstractListModel(parent)
    , interval{TimeSpan{DateTime::currentDateTime(),
                        DateTime::currentDateTime()}}
    , pomodoroService{pomodoroService}
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

void PomodoroModel::insert(const TimeSpan& timeSpan, const std::string& taskUuid)
{
    pomodoroService.registerPomodoro(timeSpan, taskUuid);
    retrieveData();
}

void PomodoroModel::remove(int row)
{
    pomodoroService.removePomodoro(storage[static_cast<size_t>(row)]);
    retrieveData();
}

void PomodoroModel::retrieveData()
{
    pomodoroService.pomodorosInTimeRange(
        interval,
        std::bind(&PomodoroModel::onDataChanged, this, std::placeholders::_1));
}

void PomodoroModel::onDataChanged(const std::vector<Pomodoro>& items)
{
    beginResetModel();
    storage = items;
    endResetModel();
}
