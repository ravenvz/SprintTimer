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
#ifndef STATISTICSWINDOWPROXY_H_VYTYRJFC
#define STATISTICSWINDOWPROXY_H_VYTYRJFC

#include <QAbstractItemModel>
#include <core/IConfig.h>
#include <core/QueryHandler.h>
#include <core/use_cases/request_sprints/RequestSprintsQuery.h>
#include <qt_gui/DatasyncRelay.h>
#include <qt_gui/SprintTimerWidget.h>
#include <qt_gui/WorkScheduleWrapper.h>
#include <qt_gui/utils/WidgetUtils.h>
#include <qt_gui/widgets/StatisticsWindow.h>

namespace sprint_timer::compose {

class StatisticsWindowProxy : public ui::qt_gui::SprintTimerWidget {
public:
    StatisticsWindowProxy(
        QueryHandler<use_cases::RequestSprintsQuery,
                     std::vector<entities::Sprint>>& requestSprintsHandler_,
        IConfig& settings_,
        QAbstractItemModel& operationRangeModel_,
        const ui::qt_gui::WorkScheduleWrapper& workScheduleWrapper_,
        ui::qt_gui::DatasyncRelay& datasyncRelay_)
        : requestSprintsHandler{requestSprintsHandler_}
        , settings{settings_}
        , operationRangeModel{operationRangeModel_}
        , workScheduleWrapper{workScheduleWrapper_}
        , datasyncRelay{datasyncRelay_}
    {
    }

    void intercept() override
    {
        if (window != nullptr && window->isVisible()) {
            ui::qt_gui::WidgetUtils::bringToForeground(window);
            return;
        }
        auto window_ = std::make_unique<ui::qt_gui::StatisticsWindow>(
            requestSprintsHandler,
            settings.firstDayOfWeek(),
            operationRangeModel,
            workScheduleWrapper,
            datasyncRelay);
        window_->setAttribute(Qt::WA_DeleteOnClose);
        window = window_.release();
        window->show();
    }

private:
    QueryHandler<use_cases::RequestSprintsQuery, std::vector<entities::Sprint>>&
        requestSprintsHandler;
    IConfig& settings;
    QAbstractItemModel& operationRangeModel;
    const ui::qt_gui::WorkScheduleWrapper& workScheduleWrapper;
    ui::qt_gui::DatasyncRelay& datasyncRelay;
    QWidget* window{nullptr};
};

} // namespace sprint_timer::compose

#endif /* end of include guard: STATISTICSWINDOWPROXY_H_VYTYRJFC */
