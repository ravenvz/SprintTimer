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
#include "mocks/CommandHandlerMock.h"
#include "mocks/HistoryMediatorMock.h"
#include "gmock/gmock.h"
#include <core/SprintTimerException.h>
#include <core/use_cases/export_data/ExportSprintsCommand.h>
#include <core/use_cases/export_data/ExportTasksCommand.h>
#include <qt_gui/presentation/DataExportPresenter.h>
#include <qt_gui/presentation/HistoryMediator.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using PresenterT = sprint_timer::ui::DataExportPresenter;
using namespace dw;
using namespace sprint_timer::ui::contracts::DataExportContract;
using namespace sprint_timer::ui;
using sprint_timer::DataFormat;
using sprint_timer::SinkType;
using sprint_timer::ui::HistoryMediator;
using sprint_timer::use_cases::ExportSprintsCommand;
using sprint_timer::use_cases::ExportTasksCommand;

namespace sprint_timer::use_cases {

bool operator==(const ExportSprintsCommand& lhs,
                const ExportSprintsCommand& rhs)
{
    return lhs.dateRange == rhs.dateRange && lhs.format == rhs.format &&
           lhs.sinkType == rhs.sinkType;
}

bool operator==(const ExportTasksCommand& lhs, const ExportTasksCommand& rhs)
{
    return lhs.dateRange == rhs.dateRange && lhs.format == rhs.format &&
           lhs.sinkType == rhs.sinkType;
}

} // namespace sprint_timer::use_cases

namespace sprint_timer::ui::contracts::DataExportContract {

bool operator==(const ExportRequestOptions& lhs,
                const ExportRequestOptions& rhs)
{
    return lhs.availableFileFormats == rhs.availableFileFormats &&
           lhs.availableSinks == rhs.availableSinks;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const ExportRequestOptions& options)
{
    os << "ExportRequestOptions{\n\tavailable formats: ";
    for (const auto& element : options.availableFileFormats) {
        os << element << ", ";
    }
    os << "\n\tavailable Sinks: ";
    for (const auto& element : options.availableSinks) {
        os << element << ", ";
    }
    os << "\n}";
    return os;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const ExportSelectedParams& params)
{
    os << "ExportSelectedParams{format: " << params.selectedFileFormat
       << ", sink: " << params.selectedSink << "}";
    return os;
}

bool operator==(const ExportSelectedParams& lhs,
                const ExportSelectedParams& rhs)
{
    return lhs.selectedFileFormat == rhs.selectedFileFormat &&
           lhs.selectedSink == rhs.selectedSink;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const ReportRequestOptions& /*options*/)
{
    os << "ReportRequestOptions{";
    os << '}';
    return os;
}

bool operator==(const ReportRequestOptions& /*lhs*/,
                const ReportRequestOptions& /*rhs*/)
{
    return true;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const ReportSelectedParams& /*params*/)
{
    os << "ReportSelectedParams{}";
    return os;
}

bool operator==(const ReportSelectedParams& /*lhs*/,
                const ReportSelectedParams& /*rhs*/)
{
    return true;
}

bool operator==(const ViewElements& lhs, const ViewElements& rhs)
{
    return std::tie(lhs.exportButtonText, lhs.reportButtonText) ==
           std::tie(rhs.exportButtonText, rhs.reportButtonText);
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const ViewElements& elements)
{
    os << "ViewElements{";
    os << elements.reportButtonText << ", ";
    os << elements.exportButtonText;
    os << '}';
    return os;
}

} // namespace sprint_timer::ui::contracts::DataExportContract

class ViewMock : public sprint_timer::ui::contracts::DataExportContract::View {
public:
    MOCK_METHOD(void,
                displayExportOptions,
                (const sprint_timer::ui::contracts::DataExportContract::
                     ExportRequestOptions&),
                (override));

    MOCK_METHOD(void,
                displayReportOptions,
                (const sprint_timer::ui::contracts::DataExportContract::
                     ReportRequestOptions&),
                (override));

    MOCK_METHOD(
        void,
        setupElements,
        (const sprint_timer::ui::contracts::DataExportContract::ViewElements&),
        (override));

    MOCK_METHOD(void, displayError, (const std::string&), (override));
};

class DataExportPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::CommandHandlerMock<
        sprint_timer::use_cases::ExportSprintsCommand>>
        exportSprintsHandlerMock;
    NiceMock<
        mocks::CommandHandlerMock<sprint_timer::use_cases::ExportTasksCommand>>
        exportTasksHandlerMock;
    NiceMock<ViewMock> viewMock;
    NiceMock<mocks::HistoryMediatorMock> mediatorMock;
    PresenterT presenter{
        exportSprintsHandlerMock, exportTasksHandlerMock, mediatorMock};
    const DateRange dateRange{current_date(), current_date()};
};

TEST_F(DataExportPresenterFixture, renames_ui_elements_when_view_is_attached)
{
    ViewElements viewElements{"Generate Report", "Export"};

    EXPECT_CALL(viewMock, setupElements(viewElements));

    presenter.attachView(viewMock);
}

TEST_F(
    DataExportPresenterFixture,
    does_nothing_when_export_requested_but_no_date_range_is_supplied_from_mediator)
{
    ON_CALL(mediatorMock, currentDateRange())
        .WillByDefault(Return(std::nullopt));
    presenter.attachView(viewMock);

    EXPECT_CALL(viewMock, displayExportOptions(_)).Times(0);

    presenter.onDataExportRequested();
}

TEST_F(
    DataExportPresenterFixture,
    does_nothing_when_report_requested_but_no_date_range_is_supplied_from_mediator)
{
    ON_CALL(mediatorMock, currentDateRange())
        .WillByDefault(Return(std::nullopt));
    presenter.attachView(viewMock);

    EXPECT_CALL(viewMock, displayReportOptions(_)).Times(0);

    presenter.onGenerateReportRequested();
}

TEST_F(DataExportPresenterFixture,
       makes_view_show_options_when_data_export_requested)
{
    ExportRequestOptions options{{"CSV", "JSON"},
                                 {"Stdout", "File", "Network"}};
    presenter.attachView(viewMock);
    ON_CALL(mediatorMock, currentDateRange()).WillByDefault(Return(dateRange));

    EXPECT_CALL(viewMock, displayExportOptions(options));

    presenter.onDataExportRequested();
}

// TODO(vizier): what should it do when no date range is provided by mediator?

TEST_F(DataExportPresenterFixture,
       makes_view_show_options_when_report_generation_requested)
{
    ReportRequestOptions options;
    presenter.attachView(viewMock);
    ON_CALL(mediatorMock, currentDateRange()).WillByDefault(Return(dateRange));

    EXPECT_CALL(viewMock, displayReportOptions(options));

    presenter.onGenerateReportRequested();
}

TEST_F(DataExportPresenterFixture,
       exports_sprint_history_when_data_export_confirmed_and_sprint_tab_active)
{
    const ExportSelectedParams selectedParams{"JSON", "Stdout"};
    const ExportSprintsCommand expected{
        dateRange, DataFormat::Json, SinkType::Stdout};
    ON_CALL(mediatorMock, currentDateRange()).WillByDefault(Return(dateRange));
    ON_CALL(mediatorMock, displayedHistory())
        .WillByDefault(
            Return(HistoryMediator::DisplayedHistory::SprintHistory));

    EXPECT_CALL(exportSprintsHandlerMock,
                handle(ExportSprintsCommand{
                    expected.dateRange, expected.format, expected.sinkType}));

    presenter.onDataExportConfirmed(selectedParams);
}

TEST_F(DataExportPresenterFixture,
       retrieves_task_history_when_data_export_confirmed_and_task_tab_active)
{
    const ExportSelectedParams selectedParams{"JSON", "Stdout"};
    const ExportTasksCommand expected{
        dateRange, DataFormat::Json, SinkType::Stdout};
    ON_CALL(mediatorMock, currentDateRange()).WillByDefault(Return(dateRange));
    ON_CALL(mediatorMock, displayedHistory())
        .WillByDefault(Return(HistoryMediator::DisplayedHistory::TaskHistory));

    EXPECT_CALL(exportTasksHandlerMock,
                handle(ExportTasksCommand{
                    expected.dateRange, expected.format, expected.sinkType}));

    presenter.onDataExportConfirmed(selectedParams);
}

TEST_F(DataExportPresenterFixture,
       makes_view_display_error_message_when_tasks_export_throws)
{
    const ExportSelectedParams selectedParams{"NonexistingFormat", "Stdout"};
    presenter.attachView(viewMock);
    ON_CALL(mediatorMock, currentDateRange()).WillByDefault(Return(dateRange));
    ON_CALL(mediatorMock, displayedHistory())
        .WillByDefault(Return(HistoryMediator::DisplayedHistory::TaskHistory));
    const std::string expectedErrorMessage{
        "Error exporting Tasks: 'actual error'"};
    ON_CALL(exportTasksHandlerMock, handle(_))
        .WillByDefault(::testing::Throw(
            sprint_timer::SprintTimerException{"actual error"}));

    EXPECT_CALL(viewMock, displayError(expectedErrorMessage));

    presenter.onDataExportConfirmed(selectedParams);
}

TEST_F(DataExportPresenterFixture,
       makes_view_display_error_message_when_sprints_export_throws)
{
    const ExportSelectedParams selectedParams{"NonexistingFormat", "Stdout"};
    presenter.attachView(viewMock);
    ON_CALL(mediatorMock, currentDateRange()).WillByDefault(Return(dateRange));
    ON_CALL(mediatorMock, displayedHistory())
        .WillByDefault(
            Return(HistoryMediator::DisplayedHistory::SprintHistory));
    const std::string expectedErrorMessage{
        "Error exporting Sprints: 'actual error'"};
    ON_CALL(exportSprintsHandlerMock, handle(_))
        .WillByDefault(::testing::Throw(
            sprint_timer::SprintTimerException{"actual error"}));

    EXPECT_CALL(viewMock, displayError(expectedErrorMessage));

    presenter.onDataExportConfirmed(selectedParams);
}

TEST_F(DataExportPresenterFixture,
       fallbacks_to_cvs_data_format_when_wrong_format_is_supplied_by_view)
{
    const ExportSelectedParams selectedParams{"NonexistingFormat", "Stdout"};
    const ExportTasksCommand expected{
        dateRange, DataFormat::Csv, SinkType::Stdout};
    ON_CALL(mediatorMock, currentDateRange()).WillByDefault(Return(dateRange));
    ON_CALL(mediatorMock, displayedHistory())
        .WillByDefault(Return(HistoryMediator::DisplayedHistory::TaskHistory));

    EXPECT_CALL(exportTasksHandlerMock,
                handle(ExportTasksCommand{
                    expected.dateRange, expected.format, expected.sinkType}));

    presenter.onDataExportConfirmed(selectedParams);
}

TEST_F(DataExportPresenterFixture,
       fallbacks_to_stdout_sink_when_wrong_sink_is_supplied_by_view)
{
    const ExportSelectedParams selectedParams{"JSON", "Gibberish"};
    const ExportSprintsCommand expected{
        dateRange, DataFormat::Json, SinkType::Stdout};
    ON_CALL(mediatorMock, currentDateRange()).WillByDefault(Return(dateRange));
    ON_CALL(mediatorMock, displayedHistory())
        .WillByDefault(
            Return(HistoryMediator::DisplayedHistory::SprintHistory));

    EXPECT_CALL(exportSprintsHandlerMock,
                handle(ExportSprintsCommand{
                    expected.dateRange, expected.format, expected.sinkType}));

    presenter.onDataExportConfirmed(selectedParams);
}
