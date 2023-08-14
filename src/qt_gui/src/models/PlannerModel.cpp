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
#include "qt_gui/models/PlannerModel.h"
#include "cpp_utils/algorithms/alg_ext.h"
#include "qt_gui/PlannerColumn.h"
#include "qt_gui/TreeItem.h"
#include "qt_gui/models/CustomRoles.h"
#include <QBrush>
#include <QColor>
#include <qicon.h>
#include <utility>

namespace {

// auto extractTags(std::span<const std::string> tags) -> QString;

// auto extractDueTime(const sprint_timer::api::TaskTimeframeDTO& timeFrame)
//     -> dw::DateTime;

// auto extractReminder(const sprint_timer::api::TaskTimeframeDTO& timeFrame)
//     -> std::optional<dw::DateTime>;

auto makeColor(std::string_view color) -> QColor;

} // namespace

namespace sprint_timer::ui::qt_gui {

using contracts::PlannerContract::PlannerItem;

PlannerModel::PlannerModel(QObject* parent_)
    : TreeModel{QStringList{"Name",
                            "Progress",
                            "Tags",
                            "Due date",
                            "Reminder",
                            "Reccurent",
                            "Type"},
                parent_}
{
}

auto PlannerModel::supportedDropActions() const -> Qt::DropActions
{
    return Qt::MoveAction;
}

auto PlannerModel::supportedDragActions() const -> Qt::DropActions
{
    return Qt::MoveAction;
}

// QVariant PlannerModel::headerData(int section,
//                                   Qt::Orientation orientation,
//                                   int role) const
// {
//     if (role != Qt::DisplayRole or orientation !=
//     Qt::Orientation::Horizontal) {
//         return QVariant{};
//     }
//     switch (section) {
//     case 0:
//         return "Name";
//     case 1:
//         return "Progress";
//     case 2:
//         return "Tags";
//     case 3:
//         return "Due date";
//     case 4:
//         return "Reminder";
//     default:
//         return QVariant{};
//     }
//
//     return QVariant{};
// }

auto PlannerModel::mimeTypes() const -> QStringList
{
    QStringList types;
    types << "application/planner.model";
    return types;
    // return QAbstractItemModel::mimeTypes();
}

auto PlannerModel::mimeData(const QModelIndexList& indexes) const -> QMimeData*
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

auto PlannerModel::dropMimeData(const QMimeData* /* data */,
                                Qt::DropAction /* action */,
                                int /* row */,
                                int /* column */,
                                const QModelIndex& /* parent */) -> bool
{
    return false;
}

auto PlannerModel::flags(const QModelIndex& index) const -> Qt::ItemFlags
{
    Qt::ItemFlags returnFlags =
        sprint_timer::ui::qt_gui::TreeModel::flags(index);

    if (!index.isValid()) {
        return Qt::ItemIsDropEnabled;
    }

    using enum PlannerColumn;

    switch (static_cast<PlannerColumn>(index.column())) {
    case Name:
        return returnFlags |= Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    case Progress:
    case Tags:
        return returnFlags | Qt::ItemIsEditable;
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

auto PlannerModel::data(const QModelIndex& index, int role) const -> QVariant
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

    return TreeModel::data(index, role);
}

auto PlannerModel::getItemData(const QModelIndex& index, int role) const -> Item
{
    auto var =
        TreeModel::data(TreeModel::index(index.row(), 0, index.parent()), role);
    return var.value<Item>();
}

auto PlannerModel::replaceItemData(const QModelIndex& index,
                                   const Item& item,
                                   int role) -> bool
{
    QVariant var;
    var.setValue(item);
    return TreeModel::setData(index, var, role);
}

auto PlannerModel::setData(const QModelIndex& index,
                           const QVariant& value,
                           int role) -> bool
{
    if (not index.isValid()) {
        return false;
    }

    using item_t = sprint_timer::ui::contracts::PlannerContract::PlannerItem;

    if (role == CustomRoles::ItemRole) {
        auto item = value.value<item_t>();
        QVariant var;
        var.setValue(makeItem(item));
        return TreeModel::setData(index, var, role);
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
        auto var = TreeModel::data(
            TreeModel::index(index.row(), 0, index.parent()), role);
        auto item = var.value<Item>();
        item.finished = !item.finished;
        QVariant nvar;
        nvar.setValue(item);
        return TreeModel::setData(index, nvar, role);
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

// auto PlannerModel::moveRows(const QModelIndex& sourceParent,
//                             int sourceRow,
//                             int count,
//                             const QModelIndex& destinationParent,
//                             int destinationChild) -> bool
// {
//     if (beginMoveRows(sourceParent,
//                       sourceRow,
//                       sourceRow + count - 1,
//                       destinationParent,
//                       destinationChild)) {
//
//         // auto sourceUuid =
//         //     sourceParent.isValid()
//         //         ?
//         sourceParent.data(CustomRoles::IdRole).value<std::string>()
//         //         : std::optional<std::string>{};
//         // auto destinationUuid = destinationParent.isValid()
//         //                            ?
//         // destinationParent.data(CustomRoles::IdRole)
//         //                                  .value<std::string>()
//         //                            : std::optional<std::string>{};
//
//         endMoveRows();
//         return true;
//     }
//     return false;
// }

// auto PlannerModel::removeRows(int row, int count, const QModelIndex&
// parent)
//     -> bool
// {
//     qDebug() << "removeRows called";
//     return false;
//
//     // beginRemoveRows(parent, row, row + count - 1);
//     // storage.removeNodes(
//     //
//     this->parent(parent).data(CustomRoles::IdRole).value<std::string>(),
//     //     row,
//     //     count);
//     // endRemoveRows();
//     // return true;
// }

// auto PlannerModel::insertRows(int row, int count, const QModelIndex&
// parent)
//     -> bool
// {
//     // if (not parent.isValid()) {
//     //     return false;
//     // }
//     //
//     // beginInsertRows(parent, row, row + count - 1);
//     // auto* parentItem = static_cast<Item*>(parent.internalPointer());
//     // if (parentItem == nullptr) {
//     //     throw std::runtime_error{"Model internal consistency failed"};
//     // }
//     // storage.addChild(, Item{}, parentItem->uuid, )
//     // endInsertRows();
//
//     return false;
//
//     // if (not parent.isValid()) {
//     //     return false;
//     // }
//     // beginInsertRows(parent, row, row + count - 1);
//     // storage.addChild(
//     //     parent.data(CustomRoles::IdRole).value<std::string>(),
//     //     static_cast<size_t>(row), 1);
//     // endInsertRows();
//     // return true;
// }

// QModelIndex
// PlannerModel::index(int row, int column, const QModelIndex& parent) const
// {
//     if (!hasIndex(row, column, parent)) {
//         return QModelIndex{};
//     }
//
//     if (!parent.isValid()) {
//         if (auto child = storage.nthChild(static_cast<size_t>(row));
//         child) {
//             return createIndex(row, column, &child.value().get());
//         }
//         return QModelIndex{};
//     }
//
//     auto* parentItem = static_cast<Item*>(parent.internalPointer());
//
//     if (auto child =
//             storage.nthChild(parentItem->uuid, static_cast<size_t>(row));
//         child) {
//         return createIndex(row, column, &child.value().get());
//     }
//
//     return QModelIndex{};
// }

// QModelIndex PlannerModel::parent(const QModelIndex& index) const
// {
//     if (!index.isValid()) {
//         return QModelIndex{};
//     }
//
//     auto* childItem = static_cast<Item*>(index.internalPointer());
//     if (childItem == nullptr) {
//         throw std::runtime_error{
//             "Model consistency failed: got nullptr instead of root"};
//     }
//
//     const auto parentInfo = findParent(childItem);
//     if (parentInfo) {
//         auto [node, row] = *parentInfo;
//         return createIndex(row, 0, node);
//     }
//
//     return QModelIndex{};
// }

// bool setData(const QModelIndex& index,
//              const QVariant& value,
//              int role = Qt::EditRole)
// {
//     return false;
// }

// bool PlannerModel::moveRows(const QModelIndex& sourceParent,
//               int sourceRow,
//               int count,
//               const QModelIndex& destinationParent,
//               int destinationChild)
// {
//     if (not sourceParent.isValid() or not destinationParent.isValid()) {
//         return false;
//     }
//
//     beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1,
//     destinationParent, destinationChild); return false;
// }

// bool removeRows(int row,
//                 int count,
//                 const QModelIndex& parent = QModelIndex{})
// {
//     return false;
// }
//
// bool insertRows(int row,
//                 int count,
//                 const QModelIndex& parent = QModelIndex{})
// {
//     return false;
// }
//

// auto PlannerModel::rowCount(const QModelIndex& parent) const -> int
// {
//     if (parent.column() > 0) {
//         return 0;
//     }
//     if (!parent.isValid()) {
//         // return storage.children().size();
//         return static_cast<int>(storage.children().size());
//     }
//     // Item* parentItem = static_cast<Item*>(parent.internalPointer());
//     auto* parentItem =
//     static_cast<api::TaskDTO*>(parent.internalPointer()); return
//     static_cast<int>(storage.children(parentItem->uuid).size());
//     // return storage.children(std::nullopt).size();
// }

// int PlannerModel::columnCount(const QModelIndex& #<{(|parent|)}>#) const
// { return 5; }

auto PlannerModel::makeItem(const PlannerItem& plannerItem) const -> Item
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

// auto PlannerModel::findParent(Item* node) const
//     -> std::optional<std::pair<const PlannerModel::Item*, int>>
// {
//     using sprint_timer::utils::and_then;
//     using sprint_timer::utils::or_else;
//     using sprint_timer::utils::transform;
//
//     auto getPayload = [this](const auto& uuid) {
//         return storage.payload(uuid);
//     };
//
//     auto getPosition = [this](const auto& uuid) {
//         return storage.positionInChildren(uuid);
//     };
//
//     // TODO replace when updgraded to C++23
//     // const auto parentUuid = storage.parent(node->uuid);
//     // const auto parent = parentUuid.and_then(getPayload);
//     // return parentUuid
//     //         .and_then(getPosition)
//     //         .transform(lambda_that_builds_result);
//
//     using sprint_timer::utils::inspect;
//
//     const auto parentUuid = storage.parent(node->uuid);
//     // std::cout << "ParentUuid: ";
//     // inspect(parentUuid,
//     //         [](const auto& uuid) { std::cout << uuid.get() <<
//     std::endl;
//     //         });
//     const auto parent = and_then(parentUuid, getPayload);
//     // std::cout << "Parent payload (ptr): ";
//     // inspect(parent, [](const auto& p) { std::cout << p.get().uuid; });
//     // std::cout << "maybeRow: ";
//     const auto maybeRow = and_then(parentUuid, getPosition);
//     // inspect(maybeRow, [](const auto& row) { std::cout << row <<
//     // std::endl;
//     // }); return transform(maybeRow, [&](size_t row) {
//     //     return std::make_pair(&parent.value().get(),
//     //     static_cast<int>(row));
//     // });
//
//     auto res = transform(maybeRow, [&](size_t row) {
//         return std::make_pair(&parent.value().get(),
//         static_cast<int>(row));
//     });
//     return res;
//     // if (res) {
//     //     std::cout << "Found parent: " res.s << std::endl;
//     // }
// }

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
