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
#ifndef SPRINTREGISTRATOR_H_RLFYANTP
#define SPRINTREGISTRATOR_H_RLFYANTP

#include "qt_gui/IndexChangedReemitter.h"
#include <QAbstractItemModel>
#include <QObject>
#include <core/CommandInvoker.h>
#include <core/ISprintStorageWriter.h>

namespace sprint_timer::ui::qt_gui {

class SprintRegistrator : public QObject {
public:
    SprintRegistrator(QAbstractItemModel& taskModel,
                      ISprintStorageWriter& sprintWriter,
                      CommandInvoker& commandInvoker,
                      IndexChangedReemitter& selectedTaskRowReemitter,
                      QObject* parent = nullptr);

    void
    onSubmissionRequested(const std::vector<dw::DateTimeRange>& timeIntervals);

    void onTaskSelected(int taskRow);

private:
    QAbstractItemModel& taskModel;
    ISprintStorageWriter& sprintWriter;
    CommandInvoker& commandInvoker;
    int candidateRow{-1};
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: SPRINTREGISTRATOR_H_RLFYANTP */

