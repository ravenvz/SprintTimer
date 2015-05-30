#ifndef MAINWINDOW_H
#define MAINWINDOW_H 

#include <QMainWindow>
#include "TaskScheduler.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(TaskScheduler scheduler, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void cancelPomodoro();
    void addTodoItem();
    void startTask();

private:
    Ui::MainWindow *ui;
    TaskScheduler taskScheduler;

    void connectSlots();
};



#endif // MAINWINDOW_H 
