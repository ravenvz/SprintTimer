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
#ifndef REORDABLELISTVIEW_H_KN6BYIAY
#define REORDABLELISTVIEW_H_KN6BYIAY

#include <QDropEvent>
#include <QListView>

namespace sprint_timer::ui::qt_gui {

/* Extension of QListView that allows items to be reordered. */
class ReordableListView : public QListView {

public:
    explicit ReordableListView(QWidget* parent);

    // Overriding to catch dropEvent and prevent model start removing rows
    // attempting drag and drop by calling another method instead.
    void dropEvent(QDropEvent* event) final;
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: REORDABLELISTVIEW_H_KN6BYIAY */
