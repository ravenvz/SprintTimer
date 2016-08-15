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

#ifdef _WIN32
// define something for Windows (32-bit and 64-bit, this part is common)
#ifdef _WIN64
// define something for Windows (64-bit only)
#endif
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#elif defined(__linux__)
// linux
#include <pwd.h>
#include <unistd.h>
#elif defined(__unix__) // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#error "Unknown compiler"
#endif

#include "QtConfig.h"
#include "core/PomodoroService.h"
#include "qt_storage_impl/QtStorageImplementersFactory.h"
#include "widgets/mainwindow.h"
#include <QApplication>
#include <cstdlib>
#include <experimental/filesystem>
#include <iostream>
#include <memory>

using std::experimental::filesystem::exists;
using std::experimental::filesystem::create_directory;

std::string getUserDataDirectory();
std::string getUnixDataDirectory();

std::string createDataDirectoryIfNotExist()
{
    std::string prefix = getUserDataDirectory();
    std::cout << "Prefix is: " << prefix << std::endl;
    const std::string dataDirectory{prefix + "/pomodoro"};
    if (!exists(dataDirectory)) {
        create_directory(dataDirectory);
    }
    return dataDirectory;
}

std::string getUserDataDirectory()
{
#ifdef _WIN_32
// TODO
#elif defined(__linux__)
    return getUnixDataDirectory();
#elif defined(__APPLE__)
// TODO
#else
#error "Unknown compiler"
#endif
}

std::string getUnixDataDirectory()
{
    if (auto xdgDataDir = std::getenv("XDG_DATA_DIR")) {
        return xdgDataDir;
    }
    std::string suffix{"/.local/share"};
    if (auto homeDir = std::getenv("HOMEE")) {
        return std::string{homeDir} + suffix;
    }
    else {
        return std::string{getpwuid(getuid())->pw_dir} + suffix;
    }
}

int main(int argc, char* argv[])
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QApplication::setOrganizationName("RavenStudio");
    QApplication::setApplicationName("Pomodoro");
    Config applicationSettings;

    const std::string dataDirectory = createDataDirectoryIfNotExist();
    QApplication app(argc, argv);
    DBService dbService{dataDirectory + "/pomodoro.db"};
//    DBService dbService{dataDirectory + "/test_pomodoro.db"};

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

    MainWindow w{applicationSettings, pomodoroService};
    w.show();

    return app.exec();
}
