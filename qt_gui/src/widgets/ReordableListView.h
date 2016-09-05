/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef REORDABLELISTVIEW_H_KN6BYIAY
#define REORDABLELISTVIEW_H_KN6BYIAY


#include "models/TaskModel.h"
#include <QDropEvent>
#include <QListView>


/* Extension of QListView that allows items to be reordered. */
class ReordableListView : public QListView {

public:
    explicit ReordableListView(QWidget* parent);

    // Overriding to catch dropEvent and prevent model start removing rows
    // attempting drag and drop by calling another method instead.
    void dropEvent(QDropEvent* event) final;


};


#endif /* end of include guard: REORDABLELISTVIEW_H_KN6BYIAY */
