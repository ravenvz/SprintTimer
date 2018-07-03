/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#define NOMINMAX // min and max macros break Howard Hinnant's date lib
#include <ShlObj.h>
#include <Windows.h>
#include <filesystem>
#if defined _WIN64
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
#include <experimental/filesystem>
#include <pwd.h>
#include <unistd.h>
#elif defined(__unix__)
#elif defined(_POSIX_VERSION)
#else
#error "Unknown compiler"
#endif

#include <core/CoreService.h>
#include <qt_storage_impl/QtStorageImplementersFactory.h>
#include <qt_gui/QtConfig.h>
#include <qt_gui/widgets/MainWindow.h>
#include <QApplication>
#include <QStyleFactory>

using std::experimental::filesystem::create_directory;
using std::experimental::filesystem::exists;

namespace {

#ifdef _WIN32
std::string getUserDataDirectory()
{
    PWSTR path;
    if (SUCCEEDED(SHGetKnownFolderPath(
            FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &path))) {
        using convert_type = std::codecvt_utf8<wchar_t>;
        const std::wstring w_path{path};
        std::wstring_convert<convert_type, wchar_t> converter;
        return converter.to_bytes(w_path);
    }
    return std::string{};
}
#elif defined(__linux__)

std::string getUserDataDirectory()
{
    if (auto xdgDataDir = std::getenv("XDG_DATA_DIR")) {
        return xdgDataDir;
    }
    const std::string suffix{"/.local/share"};
    if (auto homeDir = std::getenv("HOME")) {
        return std::string{homeDir} + suffix;
    }
    if (auto pwd = getpwuid(getuid())) {
        return std::string{pwd->pw_dir} + suffix;
    }
    return std::string{};
}

#elif defined(__APPLE__)
#error "Apple platforms are not yet supported"
#else
#error "unknown platform"
#endif

std::string getOrCreateSprintTimerDataDirectory()
{
    const std::string prefix = getUserDataDirectory();
    const std::string dataDirectory{prefix + "/sprint_timer"};
    if (!exists(dataDirectory)) {
        create_directory(dataDirectory);
    }
    return dataDirectory;
}

} // namespace


int main(int argc, char* argv[])
{
    using namespace sprint_timer;
    using namespace ui::qt_gui;
    using namespace storage::qt_storage_impl;

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QApplication::setOrganizationName("RavenStudio");
    QApplication::setApplicationName("SprintTimer");
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    Config applicationSettings;

    const std::string dataDirectory = getOrCreateSprintTimerDataDirectory();
    if (dataDirectory.empty()) {
        std::cerr << "Unable to find user data directory.";
        return 1;
    }

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

    CommandInvoker command_invoker;

    CoreService coreService{*sprintStorageReader,
                            *sprintStorageWriter,
                            *sprintYearRangeReader,
                            *taskStorageReader,
                            *taskStorageWriter,
                            *dailyDistributionReader,
                            *weeklyDistributionReader,
                            *monthlyDistributionReader,
                            command_invoker};

    sprint_timer::ui::qt_gui::MainWindow w{applicationSettings, coreService};
    w.show();
    app.setStyle(QStyleFactory::create("Fusion"));

    return app.exec();
}
