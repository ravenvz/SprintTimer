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
#include "qt_gui/presentation/DataExportContract.h"
#include "qt_gui/presentation/HistoryMediator.h"
#include <core/CommandHandler.h>
#include <core/use_cases/export_data/ExportSprintsCommand.h>
#include <core/use_cases/export_data/ExportTasksCommand.h>

namespace sprint_timer::ui {

class DataExportPresenter : public contracts::DataExportContract::Presenter,
                            public HistoryColleague {
public:
    using SprintsHandler = CommandHandler<use_cases::ExportSprintsCommand>;
    using TasksHandler = CommandHandler<use_cases::ExportTasksCommand>;
    DataExportPresenter(SprintsHandler& exportSprintsHandler,
                        TasksHandler& exportTasksHandler,
                        HistoryMediator& mediator);

    void onGenerateReportRequested() override;

    void onDataExportRequested() override;

    void updateViewImpl() override;

    void onGenerateReportConfirmed(
        const contracts::DataExportContract::ReportSelectedParams&
            selectedParams) override;

    void onDataExportConfirmed(
        const contracts::DataExportContract::ExportSelectedParams&
            selectedParams) override;

    void onSharedDataChanged() override;

private:
    SprintsHandler& exportSprintsHandler;
    TasksHandler& exportTasksHandler;
    HistoryMediator& mediator;
};

} // namespace sprint_timer::ui

