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
#include "qt_gui/widgets/DataExportWidget.h"
#include "qt_gui/dialogs/ExportDialog.h"
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace sprint_timer::ui::qt_gui {

DataExportWidget::DataExportWidget(QWidget* parent_)
    : QWidget{parent_}
{
    auto layout = std::make_unique<QVBoxLayout>();
    auto reportButton = std::make_unique<QPushButton>("Generate report");
    auto exportButton = std::make_unique<QPushButton>("Export");
    connect(exportButton.get(), &QPushButton::clicked, [this]() {
        if (auto p = presenter(); p) {
            p.value()->onDataExportRequested();
        }
    });
    layout->addWidget(reportButton.release());
    layout->addWidget(exportButton.release());
    setLayout(layout.release());
}

void DataExportWidget::displayExportOptions(
    const contracts::DataExportContract::ExportRequestOptions& options)
{
    contracts::DataExportContract::ExportSelectedParams selectedParams;
    ExportDialog dialog{options, selectedParams};
    connect(&dialog, &QDialog::accepted, [this, &selectedParams]() {
        if (auto p = presenter(); p) {
            p.value()->onDataExportConfirmed(selectedParams);
        }
    });
    dialog.setModal(true);
    dialog.exec();
}

void DataExportWidget::displayReportOptions(
    const contracts::DataExportContract::ReportRequestOptions& /*options*/)
{
}

void DataExportWidget::setupElements(
    const contracts::DataExportContract::ViewElements& /*viewElements*/)
{
}

void DataExportWidget::displayError(const std::string& errorMessage)
{
    QMessageBox messageBox;
    messageBox.setText(QString::fromStdString(errorMessage));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.exec();
}

} // namespace sprint_timer::ui::qt_gui
