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
#include "api/com_query/CommandHandler.h"
#include "api/requests/ExportSprintsCommand.h"
#include "api/requests/ExportTasksCommand.h"
#include "qt_gui/presentation/DataExportContract.h"
#include "qt_gui/presentation/HistoryMediator.h"

namespace sprint_timer::ui {

class DataExportPresenter : public contracts::DataExportContract::Presenter,
                            public HistoryColleague {
public:
    using export_sprints_hdl_t =
        asp::CommandHandler<api::ExportSprintsCommand>;
    using export_tasks_hdl_t = asp::CommandHandler<api::ExportTasksCommand>;
    DataExportPresenter(export_sprints_hdl_t& exportSprintsHandler,
                        export_tasks_hdl_t& exportTasksHandler,
                        HistoryMediator& mediator);

    void onGenerateReportRequested() override;

    void onDataExportRequested() override;

    void onGenerateReportConfirmed(
        const contracts::DataExportContract::ReportSelectedParams&
            selectedParams) override;

    void onDataExportConfirmed(
        const contracts::DataExportContract::ExportSelectedParams&
            selectedParams) override;

    void onSharedDataChanged() override;

private:
    export_sprints_hdl_t& exportSprintsHandler;
    export_tasks_hdl_t& exportTasksHandler;
    HistoryMediator& mediator;

    void updateViewImpl() override;
};

} // namespace sprint_timer::ui

