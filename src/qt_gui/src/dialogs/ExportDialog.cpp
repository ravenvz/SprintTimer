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
#include "qt_gui/dialogs/ExportDialog.h"
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QStringListModel>

namespace {

QStringList toStringList(const std::vector<std::string>& vec)
{
    QStringList result;
    std::transform(
        cbegin(vec),
        cend(vec),
        std::back_inserter(result),
        [](const auto& elem) { return QString::fromStdString(elem); });
    return result;
}

} // namespace

namespace sprint_timer::ui::qt_gui {

ExportDialog::ExportDialog(
    const contracts::DataExportContract::ExportRequestOptions& options_,
    contracts::DataExportContract::ExportSelectedParams& selectedParams_,
    QWidget* parent_)
    : QDialog{parent_}
    , selectedParams{selectedParams_}
{
    auto formLayout = std::make_unique<QFormLayout>();
    auto descriptionLabel = std::make_unique<QLabel>("Export parameters:");
    auto sinks = std::make_unique<QComboBox>();
    auto formats = std::make_unique<QComboBox>();
    sinkModel.setStringList(toStringList(options_.availableSinks));
    sinks->setModel(&sinkModel);
    formatModel.setStringList(toStringList(options_.availableFileFormats));
    formats->setModel(&formatModel);
    auto buttons = std::make_unique<QDialogButtonBox>(QDialogButtonBox::Ok |
                                                      QDialogButtonBox::Cancel);
    connect(buttons.get(), &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons.get(), &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(
        this, &QDialog::accepted, [this, f = formats.get(), s = sinks.get()]() {
            selectedParams.selectedFileFormat = f->currentText().toStdString();
            selectedParams.selectedSink = s->currentText().toStdString();
        });
    // TODO make sure that addRow takes ownership of widget
    formLayout->addRow(descriptionLabel.release());
    formLayout->addRow("&Sink:", sinks.release());
    formLayout->addRow("&Format:", formats.release());
    formLayout->addRow(buttons.release());
    setLayout(formLayout.release());
}

ExportDialog::~ExportDialog() = default;

} // namespace sprint_timer::ui::qt_gui
