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
#ifndef HISTORYTAB_H_DPZMGDNZ
#define HISTORYTAB_H_DPZMGDNZ

#include "qt_gui/delegates/HistoryItemDelegate.h"
#include "qt_gui/models/HistoryModel.h"
#include "qt_gui/presentation/HistoryContract.h"
#include <QTabWidget>

namespace sprint_timer::ui::qt_gui {

class HistoryTab : public QTabWidget, public contracts::HistoryContract::View {
public:
    HistoryTab();

    void
    displayHistory(const contracts::HistoryContract::History& history) override;

    contracts::HistoryContract::TaskEditData openEditTaskDialog(
        const contracts::HistoryContract::TaskEditData& data) override;

    contracts::HistoryContract::SprintEditData openEditSprintDialog(
        const contracts::HistoryContract::SprintEditData& data) override;

private:
    HistoryModel historyModel;
    HistoryItemDelegate delegate;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: HISTORYTAB_H_DPZMGDNZ */
