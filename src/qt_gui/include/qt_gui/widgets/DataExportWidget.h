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
#include <QWidget>

namespace sprint_timer::ui::qt_gui {

class DataExportWidget : public contracts::DataExportContract::View,
                         public QWidget {
public:
    explicit DataExportWidget(QWidget* parent = nullptr);

    void displayExportOptions(
        const contracts::DataExportContract::ExportRequestOptions& options)
        override;

    void displayReportOptions(
        const contracts::DataExportContract::ReportRequestOptions& options)
        override;

    void setupElements(const contracts::DataExportContract::ViewElements&
                           viewElements) override;

    void displayError(const std::string& errorMessage) override;
};

} // namespace sprint_timer::ui::qt_gui
