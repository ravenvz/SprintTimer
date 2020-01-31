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
#ifndef HISTORYWINDOWPROXY_H_UIWGGP91
#define HISTORYWINDOWPROXY_H_UIWGGP91

#include "ManagedDisplayable.h"
#include <QStyledItemDelegate>
#include <core/IConfig.h>
#include <core/QueryHandler.h>
#include <core/use_cases/request_sprints/RequestSprintsQuery.h>
#include <core/use_cases/request_tasks/FinishedTasksQuery.h>
#include <qt_gui/DatasyncRelay.h>
#include <qt_gui/SprintTimerWidget.h>
#include <qt_gui/models/HistoryModel.h>
#include <qt_gui/utils/WidgetUtils.h>
#include <qt_gui/widgets/HistoryWindow.h>

namespace sprint_timer::compose {

class HistoryWindowProxy : public ManagedDisplayable {
public:
    HistoryWindowProxy(
        QueryHandler<use_cases::RequestSprintsQuery,
                     std::vector<entities::Sprint>>& requestSprintsHandler_,
        QueryHandler<use_cases::FinishedTasksQuery,
                     std::vector<entities::Task>>& finishedTasksHandler_,
        ui::qt_gui::HistoryModel& historyModel_,
        QStyledItemDelegate& historyItemDelegate_,
        ui::qt_gui::DatasyncRelay& datasyncRelay_,
        QAbstractItemModel& operationRangeModel_,
        const IConfig& applicationSettings_)
        : requestSprintsHandler{requestSprintsHandler_}
        , finishedTasksHandler{finishedTasksHandler_}
        , historyModel{historyModel_}
        , historyItemDelegate{historyItemDelegate_}
        , datasyncRelay{datasyncRelay_}
        , operationRangeModel{operationRangeModel_}
        , applicationSettings{applicationSettings_}
    {
    }

private:
    QueryHandler<use_cases::RequestSprintsQuery, std::vector<entities::Sprint>>&
        requestSprintsHandler;
    QueryHandler<use_cases::FinishedTasksQuery, std::vector<entities::Task>>&
        finishedTasksHandler;
    ui::qt_gui::HistoryModel& historyModel;
    QStyledItemDelegate& historyItemDelegate;
    ui::qt_gui::DatasyncRelay& datasyncRelay;
    QAbstractItemModel& operationRangeModel;
    const IConfig& applicationSettings;

    std::unique_ptr<Displayable> create() override
    {
        return std::make_unique<ui::qt_gui::HistoryWindow>(
            requestSprintsHandler,
            finishedTasksHandler,
            historyModel,
            historyItemDelegate,
            datasyncRelay,
            operationRangeModel,
            applicationSettings.firstDayOfWeek());
    }
};

} // namespace sprint_timer::compose

#endif /* end of include guard: HISTORYWINDOWPROXY_H_UIWGGP91 */
