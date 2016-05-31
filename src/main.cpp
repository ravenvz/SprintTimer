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
