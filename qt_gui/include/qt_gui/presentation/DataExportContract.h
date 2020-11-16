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
#ifndef DATAEXPORTCONTRACT_H_JEBXZWQV
#define DATAEXPORTCONTRACT_H_JEBXZWQV

#include "qt_gui/presentation/BasePresenter.h"
#include <string>
#include <vector>

namespace sprint_timer::ui::contracts::DataExportContract {

struct ExportRequestOptions {
    std::vector<std::string> availableFileFormats;
    std::vector<std::string> availableSinks;
};

struct ExportSelectedParams {
    std::string selectedFileFormat;
    std::string selectedSink;
};

struct ReportRequestOptions {
};

struct ReportSelectedParams {
};

struct ViewElements {
    std::string reportButtonText;
    std::string exportButtonText;
};

class View {
public:
    virtual ~View() = default;

    virtual void displayExportOptions(const ExportRequestOptions& options) = 0;

    virtual void displayReportOptions(const ReportRequestOptions& options) = 0;

    virtual void setupElements(const ViewElements& viewElements) = 0;

    virtual void displayError(const std::string& errorMessage) = 0;
};

class Presenter : public BasePresenter<View> {
public:
    virtual void onGenerateReportRequested() = 0;

    virtual void onDataExportRequested() = 0;

    virtual void
    onGenerateReportConfirmed(const ReportSelectedParams& selectedParams) = 0;

    virtual void
    onDataExportConfirmed(const ExportSelectedParams& selectedParams) = 0;
};

} // namespace sprint_timer::ui::contracts::DataExportContract

#endif /* end of include guard: DATAEXPORTCONTRACT_H_JEBXZWQV */
