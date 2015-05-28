#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pomodorocanceldialog.h"
#include "addtodoitemdialog.h"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButtonCancelPom, SIGNAL(clicked(bool)), this, SLOT(cancelPomodoro()));
    connect(ui->btnAddTodo, SIGNAL(clicked(bool)), this, SLOT(addTodoItem()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::cancelPomodoro() {
    PomodoroCancelDialog cancelDialog {};
    cancelDialog.exec();
}

void MainWindow::addTodoItem() {
    AddTodoItemDialog dialog {};
    dialog.exec();
}

