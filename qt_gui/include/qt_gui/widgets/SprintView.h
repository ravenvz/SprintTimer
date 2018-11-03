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
#ifndef SPRINTVIEW_H_QBNZSNM9
#define SPRINTVIEW_H_QBNZSNM9

#include "qt_gui/models/SprintModel.h"
#include <QListView>

namespace sprint_timer::ui::qt_gui {

class SprintView : public QListView {

    Q_OBJECT

public:
    SprintView(SprintModel& sprintModel, QWidget* parent = nullptr);

    void showContextMenu(const QPoint& pos);

private:
    SprintModel& sprintModel;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: SPRINTVIEW_H_QBNZSNM9 */
