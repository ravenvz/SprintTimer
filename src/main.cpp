#include "mainwindow.h"
#include <QApplication>
#include "TaskScheduler.h"


Config readConfig() {
    Config applicationConfig {4, 1, 2, 4};
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
    QApplication a(argc, argv);
    MainWindow w {initTaskScheduler()};
    w.show();

    return a.exec();
}
