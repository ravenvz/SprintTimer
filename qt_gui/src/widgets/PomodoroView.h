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
#ifndef POMODOROVIEW_H_TMGU1KAJ
#define POMODOROVIEW_H_TMGU1KAJ

#include "dialogs/confirmationdialog.h"
#include "models/PomodoroModel.h"
#include <QListView>
#include <QMenu>

class PomodoroView : public QListView {
public:
    PomodoroView(QWidget* parent);
    QSize sizeHint() const override;

private:
    void removePomodoro();

private slots:
    void showContextMenu(const QPoint& pos);
};

#endif /* end of include guard: POMODOROVIEW_H_TMGU1KAJ */