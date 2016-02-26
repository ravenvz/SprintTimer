#include <QApplication>
#include "src/widgets/mainwindow.h"
#include "core/config.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    if (!createDbConnection()) {
        qDebug() << "FATAL unable to connect to database";
        return -1;
    }
    qDebug() << "Successfully connected to database";
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QApplication::setOrganizationName("RavenStudio");
    QApplication::setApplicationName("Pomodoro");
    Config applicationSettings;
    TaskScheduler scheduler{&applicationSettings};
    MainWindow w{scheduler, applicationSettings};
    w.show();

    return app.exec();
}
