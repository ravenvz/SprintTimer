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
#ifndef SCHEDULEMODEL_H_XC3JPGN7
#define SCHEDULEMODEL_H_XC3JPGN7

#include <QAbstractItemModel>
#include <QDate>
#include <core/IConfig.h>
#include <core/WeekSchedule.h>

namespace sprint_timer::ui::qt_gui {

class ScheduleModel : public QAbstractListModel {
public:
    ScheduleModel(const IConfig& applicationSettings,
                  QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent) const final;

    bool insertRows(int row, int count, const QModelIndex& index) final;

    bool removeRows(int row, int count, const QModelIndex& index) final;

    QVariant data(const QModelIndex& index, int role) const final;

    bool
    setData(const QModelIndex& index, const QVariant& data, int role) final;

    void sort(int column, Qt::SortOrder order) final;

private:
    const IConfig& applicationSettings;
    std::vector<QPair<QDate, WeekSchedule>> data_;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: SCHEDULEMODEL_H_XC3JPGN7 */
