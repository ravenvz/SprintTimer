#include <QApplication>
#include "gui/mainwindow.h"
#include "data_base/db_helper.h"

#include "core/TaskScheduler.h"


Config readConfig() {
    Config applicationConfig {25, 5, 15, 4};
    return applicationConfig;
}

TaskScheduler initTaskScheduler() {
    Config applicationConfig = readConfig();
    // TODO number of completed pomodoros could be non-zero 
    // It will be stored in db and should be read at this point
    unsigned completedPomodoros = 0;
    TaskScheduler taskScheduler {applicationConfig, completedPomodoros};
    return taskScheduler;
}



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (!connectToDatabase()) {
        qDebug() << "FATAL unable to connect to database";
        return -1;
    }
    qDebug() << "Successfully connected to database";
    MainWindow w {initTaskScheduler()};
    w.show();

    return app.exec();
}
