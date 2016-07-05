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
#include "QtConfig.h"
#include "core/PomodoroService.h"
#include "qt_storage_impl/QtStorageImplementersFactory.h"
#include "widgets/mainwindow.h"
#include <QApplication>
#include <memory>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    DBService dbService{"db/pomodoro.db"};
    QtStorageImplementersFactory factory{dbService};
    std::unique_ptr<IPomodoroStorageReader> pomodoroStorageReader{
        factory.createPomodoroStorageReader()};
    std::unique_ptr<IPomodoroStorageWriter> pomodoroStorageWriter{
        factory.createPomodoroStorageWriter()};
    std::unique_ptr<IPomodoroYearRangeReader> pomodoroYearRangeReader{
        factory.createPomodoroYearRangeReader()};
    std::unique_ptr<ITaskStorageReader> taskStorageReader{
        factory.createTaskStorageReader()};
    std::unique_ptr<ITaskStorageWriter> taskStorageWriter{
        factory.createTaskStorageWriter()};
    std::unique_ptr<IPomodoroDistributionReader> dailyDistributionReader{
        factory.createPomoDailyDistributionReader()};
    std::unique_ptr<IPomodoroDistributionReader> weeklyDistributionReader{
        factory.createPomoWeeklyDistributionReader()};
    std::unique_ptr<IPomodoroDistributionReader> monthlyDistributionReader{
        factory.createPomoMonthlyDistributionReader()};

    CoreApi::PomodoroService pomodoroService{*pomodoroStorageReader.get(),
                                             *pomodoroStorageWriter.get(),
                                             *pomodoroYearRangeReader.get(),
                                             *taskStorageReader.get(),
                                             *taskStorageWriter.get(),
                                             *dailyDistributionReader.get(),
                                             *weeklyDistributionReader.get(),
                                             *monthlyDistributionReader.get()};

    qDebug() << "Successfully connected to database";

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QApplication::setOrganizationName("RavenStudio");
    QApplication::setApplicationName("Pomodoro");

    Config applicationSettings;
    MainWindow w{applicationSettings, pomodoroService};
    w.show();

    return app.exec();
}
