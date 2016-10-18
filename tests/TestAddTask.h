#include <QObject>
#include <QWidget>
#include <QtTest/QtTest>
#include "widgets/MainWindow.h"

class TestAddTask : public QObject {
    Q_OBJECT

public:
    TestAddTask(QApplication* app, MainWindow* mainWindow);

private slots:
    void add_task();
    void cleanupTestCase();

private:
    QApplication* app;
    MainWindow* mainWindow;

};
