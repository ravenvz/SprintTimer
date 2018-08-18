/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#ifndef HISTORY_VIEW_H
#define HISTORY_VIEW_H

#include "qt_gui/dialogs/ExportDialog.h"
#include "qt_gui/models/HistoryModel.h"
#include "qt_gui/utils/DateInterval.h"
#include "qt_gui/widgets/DataWidget.h"
#include <QStyledItemDelegate>
#include <QTreeView>
#include <core/ISprintStorageReader.h>
#include <core/ITaskStorageReader.h>
#include <core/IYearRangeReader.h>
#include <core/QueryExecutor.h>
#include <memory>
#include <variant>

namespace Ui {
class HistoryWindow;
} // namespace Ui


namespace sprint_timer::ui::qt_gui {

class HistoryWindow : public DataWidget {
    Q_OBJECT

public:
    explicit HistoryWindow(ISprintStorageReader& sprintReader,
                           ITaskStorageReader& taskReader,
                           IYearRangeReader& sprintYearRangeReader,
                           HistoryModel& historyModel,
                           QStyledItemDelegate& historyItemDelegate,
                           QueryExecutor& queryExecutor,
                           QWidget* parent = nullptr);

    ~HistoryWindow();

    void synchronize() final;

private:
    struct ShowingSprints {
        explicit ShowingSprints(HistoryWindow& widget) noexcept;

        void retrieveHistory(HistoryWindow& widget) const;

        void
        onHistoryRetrieved(HistoryWindow& widget,
                           const std::vector<entities::Sprint>& sprints) const;

        void exportData(HistoryWindow& widget,
                        const ExportDialog::ExportOptions& options) const;
    };
    struct ShowingTasks {
        explicit ShowingTasks(HistoryWindow& widget) noexcept;

        void retrieveHistory(HistoryWindow& widget) const;

        void onHistoryRetrieved(HistoryWindow& widget,
                                const std::vector<entities::Task>& tasks) const;

        void exportData(HistoryWindow& widget,
                        const ExportDialog::ExportOptions& options) const;
    };
    using State = std::variant<std::monostate, ShowingSprints, ShowingTasks>;

    struct HistoryRequestedEvent {

        HistoryWindow& widget;

        explicit HistoryRequestedEvent(HistoryWindow& widget) noexcept;

        void operator()(std::monostate);

        void operator()(const ShowingSprints&);

        void operator()(const ShowingTasks&);
    };

    struct ExportRequestedEvent {

        HistoryWindow& widget;
        const ExportDialog::ExportOptions& options;

        ExportRequestedEvent(HistoryWindow& widget,
                             const ExportDialog::ExportOptions& options);

        void operator()(std::monostate);

        void operator()(const ShowingSprints&);

        void operator()(const ShowingTasks&);
    };

    std::unique_ptr<Ui::HistoryWindow> ui;
    ISprintStorageReader& sprintReader;
    ITaskStorageReader& taskReader;
    IYearRangeReader& sprintYearRangeReader;
    HistoryModel& historyModel;
    QueryExecutor& queryExecutor;
    State state;

    /* Assumes that history items are ordered by date ascendantly. */
    void fillHistoryModel(const HistoryModel::HistoryData& history);

    dw::TimeSpan selectedDateInterval() const;

private slots:

    void onDatePickerIntervalChanged(DateInterval newInterval);

    void onTabSelected(int tabIndex);

    void setHistoryModel(QTreeView* view);

    void onExportButtonClicked();

    void onDataExportConfirmed(const ExportDialog::ExportOptions& options);
};

} // namespace sprint_timer::ui::qt_gui

#endif // HISTORY_VIEW_H
