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

#ifndef SPRINT_TIMER_UTILS_H
#define SPRINT_TIMER_UTILS_H

#include "widgets/MainWindow.h"
#include <QString>
#include <QWidget>

namespace Simulate {

void expandMainWindow(MainWindow* mainWindow);

void expandMainWindowMenu(MainWindow* mainWindow);

void addTask(QApplication* app,
             MainWindow* mainWindow,
             const QString& name,
             int cost,
             const QString& tags);

void fillAddTaskDialog(QWidget* dialog,
                       const QString& name,
                       int cost,
                       const QString& tags);

void fillAddSprintDialog(QWidget* dialog,
                         int taskIndex,
                         const QDateTime& dateTime);

} // namespace Simulate

namespace Query {

bool listViewContains(QListView* listView, const QString& description);

} // namespace Query

namespace Assert {
void taskOutlineContainsText(QWidget* taskView, const QString& text);
void sprintOutlineContainsText(QWidget* taskView, const QString& text);
} // namespace Assert

#endif // SPRINT_TIMER_UTILS_H
