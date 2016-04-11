#include "qt_config.h"
#include "src/widgets/mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    DBService dbService{"db/pomodoro.db"};
    // if (!dbService.createDbConnection()) {
    //     qDebug() << "FATAL unable to connect to database";
    //     return -1;
    // }
    qDebug() << "Successfully connected to database";
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QApplication::setOrganizationName("RavenStudio");
    QApplication::setApplicationName("Pomodoro");
    Config applicationSettings;
    MainWindow w{applicationSettings};
    w.show();

    return app.exec();
}
