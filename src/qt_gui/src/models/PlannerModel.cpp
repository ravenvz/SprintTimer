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
#include "qt_gui/utils/DateTimeConverter.h"
#include <utility>

namespace {

QString extractTags(std::span<const std::string> tags);

} // namespace

namespace sprint_timer::ui::qt_gui {

PlannerModel::Item toPlannerItem(const use_cases::TaskNodeDTO& taskNode)
{
    using sprint_timer::utils::transform;
    const auto& task = taskNode.task;
    return PlannerModel::Item{task.uuid,
                              QString::fromStdString(task.name),
                              task.expectedCost,
                              task.actualCost,
                              task.finished,
                              extractTags(task.tags),
                              transform(taskNode.dueTime, utils::toQDateTime),
                              transform(taskNode.reminder, utils::toQDateTime),
                              taskNode.type,
                              QString::fromStdString(taskNode.notes)};
}

PlannerModel::PlannerModel(QObject* parent_)
    : QAbstractItemModel{parent_}
{
}

// Qt::DropActions supportedDropActions() const
// {
//     return Qt::MoveAction;
// }
//
// Qt::DropActions supportedDragActions() const
// {
//     return Qt::MoveAction;
// }
//
// Qt::ItemFlags flags(const QModelIndex& index) const
// {
//     if (!index.isValid() || index.model() != this) {
//         return Qt::ItemIsDropEnabled;
//     }
//     return QStandardItemModel::flags(index) | Qt::ItemIsUserCheckable |
//            Qt::ItemIsDragEnabled;
// }

QVariant PlannerModel::headerData(int section,
                                  Qt::Orientation orientation,
                                  int role) const
{
    if (role != Qt::DisplayRole or orientation != Qt::Orientation::Horizontal) {
        return QVariant{};
    }
    switch (section) {
    case 0:
        return "Name";
    case 1:
        return "Progress";
    case 2:
        return "Tags";
    case 3:
        return "Due date";
    case 4:
        return "Reminder";
    default:
        return QVariant{};
    }

    return QVariant{};
}

QVariant PlannerModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    Item* item = static_cast<Item*>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
        return item->data[index.column()];
        break;
    default:
        return {};
    }
    return {};
}

QModelIndex
PlannerModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex{};
    }

    if (!parent.isValid()) {
        if (auto child = storage.nthChild(static_cast<size_t>(row)); child) {
            return createIndex(row, column, &child.value().get());
        }
        return QModelIndex{};
    }

    Item* parentItem = static_cast<Item*>(parent.internalPointer());
    if (auto child =
            storage.nthChild(parentItem->uuid, static_cast<size_t>(row));
        child) {
        return createIndex(row, column, &child.value().get());
    }

    return QModelIndex{};
}

QModelIndex PlannerModel::parent(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return QModelIndex{};
    }

    Item* childItem = static_cast<Item*>(index.internalPointer());
    if (!childItem) {
        return QModelIndex{};
    }

    const auto parentInfo = findParent(childItem);
    if (parentInfo) {
        auto [node, row] = *parentInfo;
        return createIndex(row, 0, node);
    }

    return QModelIndex{};
}

// bool setData(const QModelIndex& index,
//              const QVariant& value,
//              int role = Qt::EditRole)
// {
//     return false;
// }
//
// bool moveRows(const QModelIndex& sourceParent,
//               int sourceRow,
//               int count,
//               const QModelIndex& destinationParent,
//               int destinationChild)
// {
//     return false;
// }
//
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
int PlannerModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0) {
        return 0;
    }
    if (!parent.isValid()) {
        return storage.children().size();
    }
    Item* parentItem = static_cast<Item*>(parent.internalPointer());
    return storage.children(parentItem->uuid).size();
    // return storage.children(std::nullopt).size();
}

int PlannerModel::columnCount(const QModelIndex& parent) const { return 5; }

void PlannerModel::displayPlanner(const use_cases::TaskTreeDTO& taskTree)
{
    beginResetModel();
    storage = taskTree.mapped<Item>(toPlannerItem);
    endResetModel();
}

void PlannerModel::displayGoals() { }

void PlannerModel::displayProjects() { }

void PlannerModel::displayReviews() { }

std::optional<std::pair<const PlannerModel::Item*, int>>
PlannerModel::findParent(Item* node) const
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

    const auto parentUuid = storage.parent(node->uuid);
    const auto parent = and_then(parentUuid, getPayload);
    const auto maybeRow = and_then(parentUuid, getPosition);
    return transform(maybeRow, [&](size_t row) {
        return std::make_pair<const Item*, int>(&parent.value().get(),
                                                static_cast<int>(row));
    });
}

PlannerModel::Item::Item(std::string uuid_,
                         QString name_,
                         int expected_,
                         int actual_,
                         bool finished_,
                         QString tags_,
                         std::optional<QDateTime> dueDate_,
                         std::optional<QDateTime> reminder_,
                         use_cases::TaskTypeDTO type_,
                         QString notes_)
    : uuid{std::move(uuid_)}
    , data{std::move(name_),
           QString{"%1/%2"}.arg(actual_).arg(expected_),
           std::move(tags_),
           dueDate_ ? dueDate_->toString() : QString{},
           reminder_ ? reminder_->toString() : QString{}}
    , finished{finished_}
    , type{type_}
    , notes{std::move(notes_)}
{
}

PlannerModel::Item::Item() = default;

PlannerModel::Item::~Item() { std::cerr << "Item destroyed " << uuid << "\n"; }

} // namespace sprint_timer::ui::qt_gui

namespace {

QString extractTags(std::span<const std::string> tags)
{
    QStringList res(static_cast<int64_t>(tags.size()));
    std::ranges::transform(tags, std::begin(res), QString::fromStdString);
    return res.join(", ");
}

} // namespace
