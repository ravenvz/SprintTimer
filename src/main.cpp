/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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

#ifdef _WIN32
#ifdef _WIN64
#endif
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
#elif TARGET_OS_IPHONE
#elif TARGET_OS_MAC
#else
#error "Unknown Apple platform"
#endif
#elif defined(__linux__)
#include <pwd.h>
#include <unistd.h>
#elif defined(__unix__)
#elif defined(_POSIX_VERSION)
#else
#error "Unknown compiler"
#endif

#include "QtConfig.h"
#include "core/CoreService.h"
#include "qt_storage_impl/QtStorageImplementersFactory.h"
#include "widgets/MainWindow.h"
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
    const std::string dataDirectory{prefix + "/sprint_timer"};
    if (!exists(dataDirectory)) {
        create_directory(dataDirectory);
    }
    return dataDirectory;
}

std::string getUserDataDirectory()
{
#ifdef _WIN_32
// TODO win32 is not yet supported
#error "Windows is not yet supported"
#elif defined(__linux__)
    return getUnixDataDirectory();
#elif defined(__APPLE__)
// TODO apple is not yet supported
#error "Apple is not yet supported"
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
    if (auto homeDir = std::getenv("HOME")) {
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
    QApplication::setApplicationName("SprintTimer");
    Config applicationSettings;

    const std::string dataDirectory = createDataDirectoryIfNotExist();
    QApplication app(argc, argv);
    DBService dbService{dataDirectory + "/sprint.db"};

    QtStorageImplementersFactory factory{dbService};
    std::unique_ptr<ISprintStorageReader> sprintStorageReader{
        factory.createSprintStorageReader()};
    std::unique_ptr<ISprintStorageWriter> sprintStorageWriter{
        factory.createSprintStorageWriter()};
    std::unique_ptr<IYearRangeReader> sprintYearRangeReader{
        factory.createYearRangeReader()};
    std::unique_ptr<ITaskStorageReader> taskStorageReader{
        factory.createTaskStorageReader()};
    std::unique_ptr<ITaskStorageWriter> taskStorageWriter{
        factory.createTaskStorageWriter()};
    std::unique_ptr<ISprintDistributionReader> dailyDistributionReader{
        factory.createSprintDailyDistributionReader()};
    std::unique_ptr<ISprintDistributionReader> weeklyDistributionReader{
        factory.createSprintWeeklyDistributionReader()};
    std::unique_ptr<ISprintDistributionReader> monthlyDistributionReader{
        factory.createSprintMonthlyDistributionReader()};

    Core::CoreService coreService{*sprintStorageReader.get(),
                                  *sprintStorageWriter.get(),
                                  *sprintYearRangeReader.get(),
                                  *taskStorageReader.get(),
                                  *taskStorageWriter.get(),
                                  *dailyDistributionReader.get(),
                                  *weeklyDistributionReader.get(),
                                  *monthlyDistributionReader.get()};

    MainWindow w{applicationSettings, coreService};
    w.show();

    return app.exec();
}
