#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogs/pomodorocanceldialog.h"
#include "dialogs/addtodoitemdialog.h"


MainWindow::MainWindow(TaskScheduler scheduler, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    taskScheduler(scheduler)
{
    ui->setupUi(this);
    connectSlots();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectSlots() {
    connect(ui->pushButtonCancelPom, SIGNAL(clicked(bool)), this, SLOT(cancelPomodoro()));
    connect(ui->btnAddTodo, SIGNAL(clicked(bool)), this, SLOT(addTodoItem()));
    connect(ui->pushButtonStart, SIGNAL(clicked(bool)), this, SLOT(startTask()));
}

void MainWindow::cancelPomodoro() {
    PomodoroCancelDialog cancelDialog {};
    cancelDialog.exec();
}

void MainWindow::addTodoItem() {
    AddTodoItemDialog dialog {};
    dialog.exec();
}

void MainWindow::startTask() {
    
}

