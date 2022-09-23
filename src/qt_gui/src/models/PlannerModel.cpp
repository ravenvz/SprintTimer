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
#include "core/utils/Algutils.h"
#include "qt_gui/TreeItem.h"
#include "qt_gui/models/CustomRoles.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include <QColor>
#include <qbrush.h>
#include <utility>

namespace {

// auto extractTags(std::span<const std::string> tags) -> QString;

// auto extractDueTime(const sprint_timer::api::TaskTimeframeDTO& timeFrame)
//     -> dw::DateTime;

// auto extractReminder(const sprint_timer::api::TaskTimeframeDTO& timeFrame)
//     -> std::optional<dw::DateTime>;

auto makeColor(std::string_view color) -> QColor;

constexpr int auxColNumber{5};

} // namespace

namespace sprint_timer::ui::qt_gui {

using contracts::PlannerContract::PlannerItem;

PlannerModel::PlannerModel(QObject* parent_)
    : TreeModel{QStringList{
                    "Name", "Progress", "Tags", "Due date", "Reminder", "Aux"},
                parent_}
{
}

auto PlannerModel::supportedDropActions() const -> Qt::DropActions
{
    return Qt::MoveAction;
}

Qt::DropActions PlannerModel::supportedDragActions() const
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

Qt::ItemFlags PlannerModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags returnFlags =
        sprint_timer::ui::qt_gui::TreeModel::flags(index) | Qt::ItemIsEditable;

    if (index.column() == 0) {
        returnFlags |= Qt::ItemIsUserCheckable;
    }

    if (!index.isValid()) {
        return Qt::ItemIsDropEnabled;
    }
    if (index.model() != this) {
        return returnFlags | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled;
    }

    return returnFlags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QVariant PlannerModel::data(const QModelIndex& index, int role) const
{
    using aux_data_t = std::tuple<std::string, bool, api::TaskTypeDTO>;

    if (!index.isValid()) {
        return {};
    }

    switch (role) {
    case CustomRoles::IdRole: {
        QVariant var;
        var.setValue(std::get<0>(
            TreeModel::data(
                this->index(index.row(), auxColNumber, index.parent()),
                Qt::DisplayRole)
                .value<aux_data_t>()));
        return var;
    }
    case Qt::DisplayRole: {
        if (index.column() == auxColNumber) {
            QVariant var;
            auto item = TreeModel::data(index, role).value<aux_data_t>();
            var.setValue(item);
            return var;
        }
        return TreeModel::data(index, Qt::DisplayRole);
    }
    case Qt::CheckStateRole: {
        return std::get<1>(
                   TreeModel::data(
                       this->index(index.row(), auxColNumber, index.parent()),
                       Qt::DisplayRole)
                       .value<aux_data_t>())
                   ? Qt::Checked
                   : Qt::Unchecked;
    }
    default:
        return {};
    }

    return {};
}

// bool PlannerModel::setData(const QModelIndex& index,
//                            const QVariant& #<{(| value |)}>#,
//                            int role)
// {
//     if (index.column() == 0 && role == Qt::CheckStateRole) {
//         auto* item = static_cast<Item*>(index.internalPointer());
//         item->finished = !item->finished;
//         emit dataChanged(index, index, QVector<int>{role});
//         // TODO propagate to presenter
//     }
//     emit dataChanged(index, index);
//     return true;
// }

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

auto PlannerModel::makeItem(const PlannerItem& dto) const -> Item
{
    // auto representDueDate =
    //     [](const std::optional<api::TaskTimeframeDTO>& maybeTimeframe,
    //        const ui::contracts::PlannerContract::Colors& colors) {
    //         using sprint_timer::utils::transform;
    //         const auto dateTime = transform(
    //             transform(maybeTimeframe, extractDueTime),
    //             utils::toQDateTime);
    //         QString repr{dateTime ? dateTime->toString("ddd dd.MM.yyyy
    //         hh:mm")
    //                               : QString{}};
    //         return ItemRepr<QString>{repr,
    //                                  makeColor(colors.dueDateColor.first),
    //                                  makeColor(colors.dueDateColor.second)};
    //     };

    // auto representReminder =
    //     [](const std::optional<api::TaskTimeframeDTO>& maybeTimeframe,
    //        const ui::contracts::PlannerContract::Colors& colors) {
    //         using sprint_timer::utils::and_then;
    //         using sprint_timer::utils::transform;
    //         const auto reminder = transform(
    //             and_then(maybeTimeframe, extractReminder),
    //             utils::toQDateTime);
    //         QString repr{reminder ? reminder->toString("ddd dd.MM.yyyy
    //         hh:mm")
    //                               : QString{}};
    //         return ItemRepr<QString>{repr,
    //                                  makeColor(colors.reminderColor.first),
    //                                  makeColor(colors.reminderColor.second)};
    //     };

    return Item{dto.uuid,
                ItemRepr{QString::fromStdString(dto.name.description),
                         makeColor(dto.name.foreground),
                         makeColor(dto.name.background)},
                ItemRepr{QString::fromStdString(dto.tags.description),
                         makeColor(dto.tags.foreground),
                         makeColor(dto.tags.background)},
                ItemRepr{QString::fromStdString(dto.progress.description),
                         makeColor(dto.progress.foreground),
                         makeColor(dto.progress.background)},
                ItemRepr{QString::fromStdString(dto.dueDate.description),
                         makeColor(dto.dueDate.foreground),
                         makeColor(dto.dueDate.background)},
                QString::fromStdString(dto.notes),
                ItemRepr{QString::fromStdString(dto.reminder.description),
                         makeColor(dto.tags.foreground),
                         makeColor(dto.tags.background)},
                dto.finished,
                dto.type};
}

auto PlannerModel::displayPlanner(
    const Tree<std::string, PlannerItem>& taskTree) -> void
{
}

auto PlannerModel::displayGoals() -> void { }

auto PlannerModel::displayProjects() -> void { }

auto PlannerModel::displayReviews() -> void { }

auto PlannerModel::findParent(Item* node) const
    -> std::optional<std::pair<const PlannerModel::Item*, int>>
{
    using sprint_timer::utils::and_then;
    using sprint_timer::utils::or_else;
    using sprint_timer::utils::transform;

    auto getPayload = [this](const auto& uuid) {
        return storage.payload(uuid);
    };

    auto getPosition = [this](const auto& uuid) {
        return storage.positionInChildren(uuid);
    };

    // TODO replace when updgraded to C++23
    // const auto parentUuid = storage.parent(node->uuid);
    // const auto parent = parentUuid.and_then(getPayload);
    // return parentUuid
    //         .and_then(getPosition)
    //         .transform(lambda_that_builds_result);

    using sprint_timer::utils::inspect;

    const auto parentUuid = storage.parent(node->uuid);
    // std::cout << "ParentUuid: ";
    // inspect(parentUuid,
    //         [](const auto& uuid) { std::cout << uuid.get() << std::endl;
    //         });
    const auto parent = and_then(parentUuid, getPayload);
    // std::cout << "Parent payload (ptr): ";
    // inspect(parent, [](const auto& p) { std::cout << p.get().uuid; });
    // std::cout << "maybeRow: ";
    const auto maybeRow = and_then(parentUuid, getPosition);
    // inspect(maybeRow, [](const auto& row) { std::cout << row <<
    // std::endl;
    // }); return transform(maybeRow, [&](size_t row) {
    //     return std::make_pair(&parent.value().get(),
    //     static_cast<int>(row));
    // });

    auto res = transform(maybeRow, [&](size_t row) {
        return std::make_pair(&parent.value().get(), static_cast<int>(row));
    });
    return res;
    // if (res) {
    //     std::cout << "Found parent: " res.s << std::endl;
    // }
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
