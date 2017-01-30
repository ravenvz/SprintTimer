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
#include <QTreeView>

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

void openHistoryView(MainWindow* mainWindow);

} // namespace Simulate

namespace Query {

bool listViewContains(QListView* listView, const QString& description);

template <typename T>
bool modelContains(QAbstractItemModel* model,
        const T& text,
        QModelIndex parent = QModelIndex())
{
    for (int r = 0; r < model->rowCount(parent); ++r) {
        QModelIndex index = model->index(r, 0, parent);
        QVariant name = model->data(index);
        if (name == text)
            return true;
        if (model->hasChildren(index)) {
            return modelContains(model, text, index);
        }
    }
    return false;
}

bool treeViewContains(QTreeView* treeView, const QString& text);

QWidget* findWidgetByName(QApplication* app, const QString& name);

} // namespace Query

namespace Assert {

void taskOutlineContainsText(QWidget* taskView, const QString& text);

void sprintOutlineContainsText(QWidget* taskView, const QString& text);

void historyContainsSprintText(QWidget* historyView, const QString& text);

void historyContainsTaskText(QWidget* historyView, const QString& text);

} // namespace Assert

#endif // SPRINT_TIMER_UTILS_H
