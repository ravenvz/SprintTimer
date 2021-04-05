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
#ifndef DATASYNCRELAY_H_DYNEFKW5
#define DATASYNCRELAY_H_DYNEFKW5

#include "qt_gui/models/AsyncListModel.h"
#include <QObject>
#include <chrono>
#include <iostream>

namespace sprint_timer::ui::qt_gui {

class DatasyncRelay : public QObject {

    Q_OBJECT

public:
    void onDataChanged() {
        displayTimes();
        clearTimes();
        starting_point = std::chrono::system_clock::now();

        emit dataUpdateRequiered(); }

    void onSyncCompleted(const std::string id) {
        exec_timepoints.push_back({id, std::chrono::system_clock::now()});
    }

    void displayTimes() const
    {
        std::cout << "Syncing times data:" << std::endl;
        for (const auto& [id, dur] : exec_timepoints) {
            std::cout << id << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(dur - starting_point).count() << " ms"  << std::endl;
        }
    }

    void clearTimes()
    {
        exec_timepoints.clear();
    }

signals:
    void dataUpdateRequiered();

private:
    using exec_time_data_t = std::pair<std::string, std::chrono::time_point<std::chrono::system_clock>>;
    std::chrono::time_point<std::chrono::system_clock> starting_point;
    std::vector<exec_time_data_t> exec_timepoints;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: DATASYNCRELAY_H_DYNEFKW5 */
