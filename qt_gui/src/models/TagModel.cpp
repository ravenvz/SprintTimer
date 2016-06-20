#include "models/TagModel.h"

TagModel::TagModel(IPomodoroService& pomodoroService, QObject* parent)
    : AsyncListModel(parent)
    , pomodoroService{pomodoroService}
{
    synchronize();
}

Qt::ItemFlags TagModel::flags(const QModelIndex& index) const
{
    return AsyncListModel::flags(index) | Qt::ItemIsEditable;
}

int TagModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(storage.size());
}

QVariant TagModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const std::string tag = storage[index.row()];

    switch (role) {
    case Qt::DisplayRole:
        return QString::fromStdString(tag);
        break;
    case Qt::EditRole:
        return QString::fromStdString(tag);
        break;
    default:
        return QVariant();
    }
}

bool TagModel::setData(const QModelIndex& index,
                       const QVariant& value,
                       int role)
{
    if (!index.isValid())
        return false;
    if (role == Qt::EditRole) {
        buffer.push_back({data(index, role).toString().toStdString(),
                          value.toString().toStdString()});
        storage[index.row()] = value.toString().toStdString();
        return true;
    }
    return false;
}

void TagModel::submitData()
{
    while (!buffer.empty()) {
        pomodoroService.editTag(buffer.back().first, buffer.back().second);
        buffer.pop_back();
    }
    requestDataUpdate();
}

void TagModel::revertData()
{
    buffer.clear();
    requestDataUpdate();
}

void TagModel::requestDataUpdate()
{
    pomodoroService.requestAllTags(
        std::bind(&TagModel::onDataArrived, this, std::placeholders::_1));
}

void TagModel::onDataArrived(const std::vector<std::string>& tags)
{
    beginResetModel();
    storage = tags;
    endResetModel();
    broadcastUpdateFinished();
}
