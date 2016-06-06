#include "models/TagModel.h"


TagModel::TagModel(IPomodoroService& pomodoroService, QObject* parent)
    : QStringListModel(parent)
    , pomodoroService{pomodoroService}
{
    requestData();
}

Qt::ItemFlags TagModel::flags(const QModelIndex& index) const
{
    return QStringListModel::flags(index) | Qt::ItemIsEditable;
}

bool TagModel::setData(const QModelIndex& index,
                       const QVariant& value,
                       int role)
{
    if (!index.isValid())
        return false;
    if (role == Qt::EditRole) {
        pomodoroService.editTag(data(index, role).toString().toStdString(),
                                value.toString().toStdString());
        QStringListModel::setData(index, value, role);
        requestData();
        return true;
    }
    return QStringListModel::setData(index, value, role);
}

void TagModel::requestData()
{
    pomodoroService.requestAllTags(
        std::bind(&TagModel::onDataArrived, this, std::placeholders::_1));
}

void TagModel::onDataArrived(const std::vector<std::string>& tags)
{
    QStringList storage;
    std::transform(
        tags.cbegin(),
        tags.cend(),
        std::back_inserter(storage),
        [](const auto& elem) { return QString::fromStdString(elem); });
    setStringList(storage);
}
