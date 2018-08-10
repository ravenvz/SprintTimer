/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#ifndef SPRINTMODEL_H_MQZ2XAPI
#define SPRINTMODEL_H_MQZ2XAPI

#include "qt_gui/models/AsyncListModel.h"
#include <core/ICoreService.h>
#include <memory>
#include <vector>

namespace sprint_timer::ui::qt_gui {

class SprintModel : public AsyncListModel {
    Q_OBJECT

public:
    SprintModel(ICoreService& coreService, QObject* parent);

    int rowCount(const QModelIndex& parent) const final;

    QVariant data(const QModelIndex& index, int role) const final;

    void insert(const dw::TimeSpan& timeSpan, const std::string& taskUuid);

    void insert(const entities::Sprint& sprint);

    void insert(const std::vector<entities::Sprint>& sprints);

    void remove(int row);

protected:
    void requestDataUpdate() final;

private:
    std::vector<entities::Sprint> storage;
    dw::TimeSpan interval;
    ICoreService& coreService;

    void onDataChanged(const std::vector<entities::Sprint>& items);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: SPRINTMODEL_H_MQZ2XAPI */
