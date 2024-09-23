/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "qt_gui/models/PlannerTreeModel.h"
#include "qt_gui/PlannerColumn.h"

namespace {

// auto extractTags(std::span<const std::string> tags) -> QString
// {
//     QStringList res(static_cast<int64_t>(tags.size()));
//     std::ranges::transform(tags, std::begin(res), QString::fromStdString);
//     return res.join(", ");
// }
//
// auto extractDueTime(const sprint_timer::api::TaskTimeframeDTO& timeFrame)
//     -> dw::DateTime
// {
//     return timeFrame.frame.finish();
// }
//
// auto extractReminder(const sprint_timer::api::TaskTimeframeDTO& timeFrame)
//     -> std::optional<dw::DateTime>
// {
//     return timeFrame.remindAt;
// }

auto makeColor(std::string_view color) -> QColor;

} // namespace

namespace sprint_timer::ui::qt_gui {

PlannerTreeModel::PlannerTreeModel(QObject* parent)
    : QAbstractItemModel{parent}
{
}

PlannerTreeModel::~PlannerTreeModel() { }

auto PlannerTreeModel::supportedDropActions() const -> Qt::DropActions
{
    return Qt::MoveAction;
}

auto PlannerTreeModel::supportedDragActions() const -> Qt::DropActions
{
    return Qt::MoveAction;
}

auto PlannerTreeModel::rowCount(const QModelIndex& parent) const -> int
{
    storage.children(storage.cend()).size();
}

auto PlannerTreeModel::columnCount(const QModelIndex& parent) const -> int
{
    return 6;
}

auto PlannerTreeModel::mimeTypes() const -> QStringList
{
    QStringList types;
    types << "application/planner.model";
    return types;
}

auto PlannerTreeModel::mimeData(const QModelIndexList& indexes) const
    -> QMimeData*
{
    auto mimeData = std::make_unique<QMimeData>();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    for (const auto& index : indexes) {
        if (index.isValid()) {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << text;
        }
    }

    mimeData->setData("application/planner.model", encodedData);

    return mimeData.release();
}

auto PlannerTreeModel::dropMimeData(const QMimeData* /* data */,
                                    Qt::DropAction /* action */,
                                    int /* row */,
                                    int /* column */,
                                    const QModelIndex& /* parent */) -> bool
{
    return false;
}

auto PlannerTreeModel::flags(const QModelIndex& index) const -> Qt::ItemFlags
{
    Qt::ItemFlags returnFlags;
    // Qt::ItemFlags returnFlags = QAbstractItemModel::flags(index);
    // sprint_timer::ui::qt_gui::TreeModel::flags(index);

    if (!index.isValid()) {
        return Qt::ItemIsDropEnabled;
    }

    using enum PlannerColumn;

    switch (static_cast<PlannerColumn>(index.column())) {
    case Name:
        return returnFlags |= Qt::ItemIsUserCheckable;
    case Progress:
    case Tags:
        return returnFlags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled |
               Qt::ItemIsEditable;
    case DueDate:
    case Reminder:
    case Recurrent:
    case Type:
    default:
        return Qt::NoItemFlags;
    }

    // if (index.model() != this) {
    //     return returnFlags | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled |
    //            Qt::ItemIsEditable;
    // }

    return Qt::NoItemFlags;
    // return returnFlags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled |
    //        Qt::ItemIsEditable;
}

auto PlannerTreeModel::data(const QModelIndex& index, int role) const
    -> QVariant
{
    if (not index.isValid()) {
        return {};
    }

    auto item = getItemData(index, role);

    using enum PlannerColumn;

    ItemRepr repr;

    switch (static_cast<PlannerColumn>(index.column())) {
    case Name:
        repr = item.name;
        break;
    case Tags:
        repr = item.tags;
        break;
    case Progress:
        repr = item.progress;
        break;
    case DueDate:
        repr = item.dueDate;
        break;
    case Reminder:
        repr = item.reminder;
        break;
    case Recurrent:
        return item.recurrent;
    case Type: {
        QVariant typeVar;
        typeVar.setValue(item.type);
        return typeVar;
    }
    default:
        return {};
    }

    switch (role) {
    case Qt::DisplayRole:
        return repr.payload;
    case Qt::ForegroundRole:
        return repr.foreground;
    case Qt::BackgroundRole:
        return repr.background;
    case Qt::CheckStateRole:
        return item.finished ? Qt::Checked : Qt::Unchecked;
    case Qt::DecorationRole: {
        if (static_cast<PlannerColumn>(index.column()) == PlannerColumn::Name) {
            if (item.recurrent) {
                return QIcon{":icons/recurrence.png"};
            }
            if (item.type == api::TaskTypeDTO::Folder) {
                return QIcon{":icons/folder.png"};
            }
        }
        break;
    }

    case CustomRoles::IdRole: {
        QVariant uuid;
        uuid.setValue(item.uuid);
        return uuid;
    }

    case Qt::EditRole: {
        QVariant details;
        details.setValue(std::tuple<std::string, QString, QString, QString>{
            item.uuid,
            item.name.payload,
            item.progress.payload,
            item.tags.payload});
        return details;
    }

    default:
        return {};
    }

    return {};
}

auto PlannerTreeModel::getItemData(const QModelIndex& index, int role) const
    -> Item
{
    auto* item = static_cast<Item*>(index.internalPointer());
    return *item;
    // auto var =
    //     TreeModel::data(TreeModel::index(index.row(), 0, index.parent()),
    //     role);
    // return var.value<Item>();
}

auto PlannerTreeModel::replaceItemData(const QModelIndex& index,
                                       const Item& item,
                                       int role) -> bool
{
    QVariant var;
    var.setValue(item);
    return true;
    // return TreeModel::setData(index, var, role);
}

auto PlannerTreeModel::setData(const QModelIndex& index,
                               const QVariant& value,
                               int role) -> bool
{
    if (not index.isValid()) {
        return false;
    }

    using item_t = sprint_timer::ui::contracts::PlannerContract::PlannerItem;

    if (role == CustomRoles::ItemRole) {
        return {};
        // auto item = value.value<item_t>();
        // QVariant var;
        // var.setValue(makeItem(item));
        // return TreeModel::setData(index, var, role);
    }

    using enum PlannerColumn;

    if (role == Qt::EditRole) {
        switch (static_cast<PlannerColumn>(index.column())) {
        case Name: {
            auto item = getItemData(index, role);
            const auto updatedName = value.toString();
            if (item.name.payload == updatedName) {
                return false;
            }
            item.name.payload = updatedName;
            return replaceItemData(index, item, role);
        }
        case Progress: {
            auto item = getItemData(index, role);
            const auto updatedEstimation = value.toInt();
            const auto payloadParts = item.progress.payload.split('/');
            int actual = payloadParts.front().toInt();
            int estimated = payloadParts.front().toInt();
            if (estimated == updatedEstimation) {
                return false;
            }
            item.progress.payload =
                QString{"%1/%2"}.arg(actual).arg(updatedEstimation);
            qDebug() << "Payload = " << item.progress.payload;
            return replaceItemData(index, item, role);
        }
        case Tags: {
            auto item = getItemData(index, role);
            const auto updatedTags = value.toString();
            if (item.tags.payload == updatedTags) {
                return false;
            }
            item.tags.payload = updatedTags;
            return replaceItemData(index, item, role);
        }
        default:
            return false;
        }
    }

    if (role == Qt::CheckStateRole) {
        return {};
        // auto var = TreeModel::data(
        //     TreeModel::index(index.row(), 0, index.parent()), role);
        // auto item = var.value<Item>();
        // item.finished = !item.finished;
        // QVariant nvar;
        // nvar.setValue(item);
        // return TreeModel::setData(index, nvar, role);
    }

    // if (role != Qt::EditRole) {
    //     return false;
    // }

    // if (TreeModel::setData(index, value, role)) {
    //     emit dataChanged(index, index);
    //     return true;
    // }

    // if (index.column() == 0 && role == Qt::EditRole) {
    //     auto* item = static_cast<Item*>(index.internalPointer());
    //     item->finished = !item->finished;
    //     // emit dataChanged(index, index, QVector<int>{role});
    //     // TODO propagate to presenter
    //     std::cout << "HERE" << std::endl;
    //     emit dataChanged(index, index);
    //     return true;
    // }

    return false;
}

auto PlannerTreeModel::index(int row,
                             int column,
                             const QModelIndex& parent) const -> QModelIndex
{
    if (not hasIndex(row, column, parent)) {
        return QModelIndex{};
    }

    if (not parent.isValid()) {
        auto children = storage.children(storage.end());
        if (row < children.size()) {
            return createIndex(row, column, &children[row]);
        }
        return QModelIndex{};
    }

    auto* parentItem = static_cast<Item*>(parent.internalPointer());
    if (parentItem == nullptr) {
        throw std::runtime_error{"Invalid internal pointer"};
    }

    auto node = std::find_if(storage.begin(), storage.end(), [parentItem](const auto& item) {
        return item.uuid == parentItem->uuid;
    });
    auto children = storage.children(node)[row];
    if (row < children.size()) {
        return createIndex(row, column, &children[row]);
    }

    return QModelIndex{};
}

auto PlannerTreeModel::parent(const QModelIndex& index) const
{
    if (not index.isValid()) {
        return QModelIndex{};
    }

    auto* childItem = static_cast<Item*>(index.internalPointer());
    if (childItem == nullptr) {
        throw std::runtime_error{
            "Model consistency failed: got nullptr instead of root"};
    }

    const auto parentInfo = storage.
}

auto PlannerTreeModel::makeItem(
    const contracts::PlannerContract::Item& plannerItem) const -> Item
{
    auto make_repr = [](const auto& item) {
        return ItemRepr{.payload = QString::fromStdString(item.description),
                        .foreground = QBrush{makeColor(item.foreground)},
                        .background = QBrush{makeColor(item.background)}};
    };
    // std::cout << plannerItem.name.description << " type: " <<
    // plannerItem.type
    //           << std::endl;
    return Item{plannerItem.uuid,
                make_repr(plannerItem.name),
                make_repr(plannerItem.tags),
                make_repr(plannerItem.progress),
                make_repr(plannerItem.dueDate),
                QString::fromStdString(plannerItem.notes),
                make_repr(plannerItem.reminder),
                plannerItem.finished,
                plannerItem.recurrent,
                plannerItem.type};
}

} // namespace sprint_timer::ui::qt_gui

namespace {

// auto extractTags(std::span<const std::string> tags) -> QString
// {
//     QStringList res(static_cast<int64_t>(tags.size()));
//     std::ranges::transform(tags, std::begin(res), QString::fromStdString);
//     return res.join(", ");
// }
//
// auto extractDueTime(const sprint_timer::api::TaskTimeframeDTO& timeFrame)
//     -> dw::DateTime
// {
//     return timeFrame.frame.finish();
// }
//
// auto extractReminder(const sprint_timer::api::TaskTimeframeDTO& timeFrame)
//     -> std::optional<dw::DateTime>
// {
//     return timeFrame.remindAt;
// }

auto makeColor(std::string_view color) -> QColor
{
    return QColor{QString::fromStdString(std::string{color})};
};

} // namespace
