/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "qt_gui/models/ScheduleModel.h"
#include <QMetaType>
#include <QString>

Q_DECLARE_METATYPE(sprint_timer::WeekSchedule)

namespace {

QString describeSchedule(const sprint_timer::WeekSchedule& schedule,
                         dw::Weekday firstDayOfWeek);



} // namespace

namespace sprint_timer::ui::qt_gui {

ScheduleModel::ScheduleModel(const IConfig& applicationSettings_,
                             QObject* parent_)
    : QAbstractListModel{parent_}
    , applicationSettings{applicationSettings_}
{
}

int ScheduleModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(data_.size());
}

bool ScheduleModel::insertRows(int row, int count, const QModelIndex& index)
{
    using namespace dw;
    beginInsertRows(index, row, row + count - 1);
    data_.insert(data_.begin() + row, count, {QDate{}, WeekSchedule{}});
    endInsertRows();
    return true;
}

bool ScheduleModel::removeRows(int row, int count, const QModelIndex& index)
{
    beginRemoveRows(index, row, row + count - 1);
    data_.erase(data_.begin() + row, data_.begin() + row + count);
    endRemoveRows();
    return true;
}

QVariant ScheduleModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto& [date, schedule] = data_[static_cast<size_t>(index.row())];
    const auto& entry = data_[static_cast<size_t>(index.row())];

    switch (role) {
    case Qt::DisplayRole: {
        const QString scheduleDescription{
            describeSchedule(schedule, applicationSettings.firstDayOfWeek())};
        return QString{"%1 %2"}
            .arg(date.toString("dd.MM.yyyy"))
            .arg(scheduleDescription);
    }
    case Qt::EditRole: {
        QVariant ventry;
        ventry.setValue(entry);
        return ventry;
    }
    default:
        return QVariant();
    }
}

bool ScheduleModel::setData(const QModelIndex& index,
                            const QVariant& data,
                            int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;
    auto row = index.row();
    data_[row] = data.value<QPair<QDate, WeekSchedule>>();
    QVector<int> roles;
    roles << role;
    emit dataChanged(index, index, roles);
    return true;
}

void ScheduleModel::sort(int column, Qt::SortOrder order)
{
    emit layoutAboutToBeChanged();
    if (order == Qt::AscendingOrder)
        std::sort(
            data_.begin(), data_.end(), [](const auto& lhs, const auto& rhs) {
                return lhs.first < rhs.first;
            });
    else
        std::sort(
            data_.begin(), data_.end(), [](const auto& lhs, const auto& rhs) {
                return lhs.first > rhs.first;
            });
    emit layoutChanged();
}

} // namespace sprint_timer::ui::qt_gui

namespace {

QString describeSchedule(const sprint_timer::WeekSchedule& schedule,
                         dw::Weekday firstDayOfWeek)
{
    if (firstDayOfWeek == dw::Weekday::Monday)
        return QString{"%1, %2, %3, %4, %5, %6, %7"}
            .arg(schedule.targetGoal(dw::Weekday::Monday))
            .arg(schedule.targetGoal(dw::Weekday::Tuesday))
            .arg(schedule.targetGoal(dw::Weekday::Wednesday))
            .arg(schedule.targetGoal(dw::Weekday::Thursday))
            .arg(schedule.targetGoal(dw::Weekday::Friday))
            .arg(schedule.targetGoal(dw::Weekday::Saturday))
            .arg(schedule.targetGoal(dw::Weekday::Sunday));
    return QString{"%1, %2, %3, %4, %5, %6, %7"}
        .arg(schedule.targetGoal(dw::Weekday::Sunday))
        .arg(schedule.targetGoal(dw::Weekday::Monday))
        .arg(schedule.targetGoal(dw::Weekday::Tuesday))
        .arg(schedule.targetGoal(dw::Weekday::Wednesday))
        .arg(schedule.targetGoal(dw::Weekday::Thursday))
        .arg(schedule.targetGoal(dw::Weekday::Friday))
        .arg(schedule.targetGoal(dw::Weekday::Saturday));
}

} // namespace
