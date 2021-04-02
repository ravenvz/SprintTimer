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
#include "qt_gui/presentation/DataExportPresenter.h"
#include "core/SprintTimerException.h"
#include <core/DataExporter.h>

namespace {

// TODO replace with compile-time data
const std::vector<std::string> availableSinks{"Stdout", "File", "Network"};
const std::vector<std::string> availableFileFormats{"CSV", "JSON"};

sprint_timer::DataFormat translateFormats(const std::string& format);

sprint_timer::SinkType translateSinks(const std::string& sink);

std::pair<sprint_timer::DataFormat, sprint_timer::SinkType> translateParams(
    const sprint_timer::ui::contracts::DataExportContract::ExportSelectedParams&
        params);

std::string composeErrorMessage(
    sprint_timer::SprintTimerException& exc,
    sprint_timer::ui::HistoryMediator::DisplayedHistory displayedHistory);

} // namespace

namespace sprint_timer::ui {

DataExportPresenter::DataExportPresenter(
    export_sprints_hdl_t& exportSprintsHandler_,
    export_tasks_hdl_t& exportTasksHandler_,
    HistoryMediator& mediator_)
    : exportSprintsHandler{exportSprintsHandler_}
    , exportTasksHandler{exportTasksHandler_}
    , mediator{mediator_}
{
}

void DataExportPresenter::onGenerateReportRequested()
{
    if (auto v = view(); v && mediator.currentDateRange()) {
        contracts::DataExportContract::ReportRequestOptions options;
        v.value()->displayReportOptions(options);
    }
}

void DataExportPresenter::onDataExportRequested()
{
    if (auto v = view(); v && mediator.currentDateRange()) {
        contracts::DataExportContract::ExportRequestOptions options{
            availableFileFormats, availableSinks};
        v.value()->displayExportOptions(options);
    }
}

void DataExportPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        contracts::DataExportContract::ViewElements viewElements{
            "Generate Report", "Export"};
        v.value()->setupElements(viewElements);
    }
}

void DataExportPresenter::onViewAttached() { updateView(); }

void DataExportPresenter::onGenerateReportConfirmed(
    const contracts::DataExportContract::ReportSelectedParams& selectedParams)
{
}

void DataExportPresenter::onDataExportConfirmed(
    const contracts::DataExportContract::ExportSelectedParams& selectedParams)
{
    const auto [dataFormat, sinkType] = translateParams(selectedParams);
    const auto displayedHistory = mediator.displayedHistory();
    try {
        switch (displayedHistory) {
        case HistoryMediator::DisplayedHistory::SprintHistory: {

            exportSprintsHandler.handle(use_cases::ExportSprintsCommand{
                *mediator.currentDateRange(), dataFormat, sinkType});
            break;
        }
        case HistoryMediator::DisplayedHistory::TaskHistory: {
            exportTasksHandler.handle(use_cases::ExportTasksCommand{
                *mediator.currentDateRange(), dataFormat, sinkType});

        } break;
        }
    }
    catch (SprintTimerException& exc) {
        if (auto v = view(); v) {
            v.value()->displayError(composeErrorMessage(exc, displayedHistory));
        }
    }
}

void DataExportPresenter::onSharedDataChanged() { }

} // namespace sprint_timer::ui

namespace {

sprint_timer::DataFormat translateFormats(const std::string& format)
{
    using sprint_timer::DataFormat;
    if (format == "JSON")
        return DataFormat::Json;
    return DataFormat::Csv;
};

sprint_timer::SinkType translateSinks(const std::string& sink)
{
    using sprint_timer::SinkType;
    if (sink == "Network")
        return SinkType::Network;
    if (sink == "File")
        return SinkType::File;
    return SinkType::Stdout;
};

std::pair<sprint_timer::DataFormat, sprint_timer::SinkType> translateParams(
    const sprint_timer::ui::contracts::DataExportContract::ExportSelectedParams&
        params)
{
    return std::make_pair(translateFormats(params.selectedFileFormat),
                          translateSinks(params.selectedSink));
};

std::string composeErrorMessage(
    sprint_timer::SprintTimerException& exc,
    sprint_timer::ui::HistoryMediator::DisplayedHistory displayedHistory)
{
    using sprint_timer::ui::HistoryMediator;
    std::stringstream ss;
    ss << "Error exporting ";
    ss << (displayedHistory == HistoryMediator::DisplayedHistory::SprintHistory
               ? "Sprints"
               : "Tasks");
    ss << ": '" << exc.what() << "'";
    return ss.str();
}

} // namespace
