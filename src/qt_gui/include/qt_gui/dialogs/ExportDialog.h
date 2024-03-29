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
#ifndef SPRINT_TIMER_EXPORTDIALOG_H
#define SPRINT_TIMER_EXPORTDIALOG_H

#include "qt_gui/presentation/DataExportContract.h"
#include <QDialog>
#include <QStringListModel>
#include <memory>

namespace sprint_timer::ui::qt_gui {

class ExportDialog : public QDialog {
public:
    ExportDialog(
        const contracts::DataExportContract::ExportRequestOptions& options,
        contracts::DataExportContract::ExportSelectedParams& selectedParams,
        QWidget* parent = nullptr);

    ~ExportDialog() override;

private:
    contracts::DataExportContract::ExportSelectedParams& selectedParams;
    QStringListModel sinkModel;
    QStringListModel formatModel;
};

} // namespace sprint_timer::ui::qt_gui

#endif // SPRINT_TIMER_EXPORTDIALOG_H
