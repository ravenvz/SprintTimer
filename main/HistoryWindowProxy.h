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

#include "DisplayableLifestyleProxy.h"
#include <QStyledItemDelegate>
#include <core/IConfig.h>
#include <qt_gui/models/HistoryModel.h>
#include <qt_gui/presentation/HistoryPresenter.h>
#include <qt_gui/utils/WidgetUtils.h>
#include <qt_gui/widgets/DataExportWidget.h>
#include <qt_gui/widgets/DateRangeSelector.h>
#include <qt_gui/widgets/HistoryTab.h>
#include <qt_gui/widgets/HistoryWindow.h>

namespace sprint_timer::compose {

class HistoryWindowProxy : public DisplayableLifestyleProxy {
public:
    HistoryWindowProxy(
        ui::contracts::DateRangeSelectorContract::Presenter&
            dateRangeSelectorPresenter_,
        ui::contracts::HistoryContract::Presenter& historyPresenter_,
        ui::contracts::DataExportContract::Presenter& dataExportPresenter_)
        : dateRangeSelectorPresenter{dateRangeSelectorPresenter_}
        , historyPresenter{historyPresenter_}
        , dataExportPresenter{dataExportPresenter_}
    {
    }

private:
    ui::contracts::DateRangeSelectorContract::Presenter&
        dateRangeSelectorPresenter;
    ui::contracts::HistoryContract::Presenter& historyPresenter;
    ui::contracts::DataExportContract::Presenter& dataExportPresenter;

    std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget> create() override
    {
        using namespace ui::qt_gui;
        return std::make_unique<ui::qt_gui::HistoryWindow>(
            std::make_unique<DateRangeSelector>(dateRangeSelectorPresenter),
            std::make_unique<DataExportWidget>(dataExportPresenter),
            std::make_unique<HistoryTab>(historyPresenter));
    }
};

} // namespace sprint_timer::compose

#endif /* end of include guard: HISTORYWINDOWPROXY_H_UIWGGP91 */
