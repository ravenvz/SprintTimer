#include "QtConfig.h"
#include "core/CoreService.h"
#include "qt_storage_impl/QtStorageImplementersFactory.h"
#include "widgets/MainWindow.h"
#include <QApplication>
#include <cstdlib>
#include <iostream>
#include <memory>
#include "TestAddTask.h"

int main(int argc, char* argv[])
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QApplication::setOrganizationName("RavenStudio");
    QApplication::setApplicationName("TestSprintTimer");
    Config applicationSettings;

    QApplication app(argc, argv);
    DBService dbService{QString{"sprint_test.db"}};

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

    QTest::qExec(new TestAddTask{&app, &w}, argc, argv);

    return 0;
}
