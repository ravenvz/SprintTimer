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

#include "Utils.h"
#include <QtTest/QtTest>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTimeEdit>

namespace Simulate {

void addTask(QApplication* app,
             MainWindow* mainWindow,
             const QString& name,
             int cost,
             const QString& tags)
{
    Simulate::expandMainWindow(mainWindow);
    auto addTaskButton = mainWindow->findChild<QPushButton*>("pbAddTask");
    QTest::mouseClick(addTaskButton, Qt::MouseButton::LeftButton);
    QTest::qWait(500);
    Simulate::fillAddTaskDialog(app->activeWindow(), name, cost, tags);
}

void expandMainWindow(MainWindow* mainWindow)
{
    auto expandButton = mainWindow->findChild<QPushButton*>("pbToggleView");
    QTest::mouseClick(expandButton, Qt::MouseButton::LeftButton);
    QTest::qWait(500);
}

void expandMainWindowMenu(MainWindow* mainWindow)
{
    auto expandButton = mainWindow->findChild<QPushButton*>("pbToggleMenu");
    QTest::mouseClick(expandButton, Qt::MouseButton::LeftButton);
    QTest::qWait(500);
}

void fillAddTaskDialog(QWidget* dialog,
                       const QString& name,
                       int cost,
                       const QString& tags)
{
    auto taskName = dialog->findChild<QLineEdit*>("taskName");
    taskName->setText(name);
    auto estimatedCost = dialog->findChild<QSpinBox*>("estimatedCost");
    estimatedCost->setValue(cost);
    auto tagsInput = dialog->findChild<QLineEdit*>("leTags");
    tagsInput->setText(tags);
    auto okButton = dialog->findChild<QDialogButtonBox*>("buttonBox");
    QTest::qWait(1000);
    okButton->accepted();
}

void fillAddSprintDialog(QWidget* dialog,
                         int taskIndex,
                         const QDateTime& dateTime)
{
    auto timeEdit = dialog->findChild<QTimeEdit*>("timeEditSprintStartTime");
    timeEdit->setDateTime(dateTime);
    auto okButton
        = dialog->findChild<QDialogButtonBox*>("buttonBoxConfirmOrCancel");
    okButton->accepted();
}

} // namespace Simulate

namespace Query {

bool listViewContains(QListView* listView, const QString& description)
{
    auto model = listView->model();

    for (int i = 0; i < model->rowCount(); ++i) {
        if (description == model->data(model->index(i, 0)).toString())
            return true;
    }

    return false;
}

} // namespace Query

namespace Assert {

void taskOutlineContainsText(QWidget* taskOutline, const QString& text)
{
    auto taskList = taskOutline->findChild<QListView*>("lvTaskView");
    if (!taskList)
        QFAIL("Task list not found");
    QVERIFY(Query::listViewContains(taskList, text));
}

void sprintOutlineContainsText(QWidget* sprintOutline, const QString& text)
{
    auto taskList = sprintOutline->findChild<QListView*>("lvFinishedSprints");
    QVERIFY(Query::listViewContains(taskList, text));
}

} // namespace Assert
