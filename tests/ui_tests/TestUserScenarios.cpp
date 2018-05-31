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

#include "TestUserScenarios.h"
#include "Utils.h"


TestUserScenarios::TestUserScenarios(QApplication* app, MainWindow* mainWindow)
    : TestCase{app, mainWindow}
{
}


void TestUserScenarios::add_task()
{
    auto taskOutline = mainWindow->findChild<QWidget*>("TaskOutline");
    if (!taskOutline) {
        QFAIL("Can't find task outline");
        return;
    }

    Simulate::addTask(app, mainWindow, "Test task", 4, "Tag1 Tag2");
    QTest::qWait(5000);

    Assert::taskOutlineContainsText(taskOutline, "#Tag1 #Tag2 Test task 0/4");
}

void TestUserScenarios::manual_add_sprint()
{
    Simulate::expandMainWindowMenu(mainWindow);
    auto addSprintButton
        = mainWindow->findChild<QWidget*>("pbAddSprintManually");
    QTest::mouseClick(addSprintButton, Qt::MouseButton::LeftButton);
    QTest::qWait(500);

    Simulate::fillAddSprintDialog(
        app->activeWindow(), 0, QDateTime{QDate::currentDate(), QTime{12, 50}});
    auto sprintOutline = mainWindow->findChild<QWidget*>("SprintOutline");
    auto taskOutline = mainWindow->findChild<QWidget*>("TaskOutline");
    QTest::qWait(500);

    Assert::taskOutlineContainsText(taskOutline, "#Tag1 #Tag2 Test task 1/4");
    Assert::sprintOutlineContainsText(sprintOutline,
            "12:50 - 13:15 #Tag1 #Tag2 Test task");
    QTest::qWait(500);

    Simulate::openHistoryView(mainWindow);
    QTest::qWait(500);

    auto historyView = Query::findWidgetByName(app, "HistoryWindow");

    Assert::historyContainsSprintText(historyView, "12:50 - 13:15 #Tag1 #Tag2 Test task");
    QTest::qWait(5000);
}
