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
#include "gtest/gtest.h"
#include <QCoreApplication>
#include <QTimer>

int main(int argc, char** argv)
{
    // QCoreApplication app{argc, argv};
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    // QTimer exitTimer;
    // QObject::connect(
    //     &exitTimer, &QTimer::timeout, &app, QCoreApplication::quit);
    // exitTimer.start(std::chrono::seconds{5});
    // exitTimer.start();
    // app.exec();
    return ret;
}
