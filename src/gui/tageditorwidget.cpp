#include "tageditorwidget.h"
#include "ui_tageditor.h"
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>


TagEditorWidget::TagEditorWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TagEditorWidget)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    model = new QSqlTableModel();
    model->setTable("tag");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setSort(1, Qt::AscendingOrder);
    model->select();
    ui->listViewTags->setModel(model);
    ui->listViewTags->setModelColumn(1);
    connect(ui->buttonBoxConfirm, SIGNAL(accepted()), this, SLOT(onAccept()));
    connect(ui->buttonBoxConfirm, SIGNAL(rejected()), this, SLOT(onReject()));
}

TagEditorWidget::~TagEditorWidget() {
    delete model;
    delete ui;
}

void TagEditorWidget::onAccept() {
    model->submitAll();
    if (model->lastError().isValid()) {
        // TODO check for specific error for unique constraint violation
        QMessageBox msgBox;
        msgBox.setText("Tag name must be unique.");
        msgBox.exec();
        return;
    }
    emit dataSetChanged();
    this->close();
}

void TagEditorWidget::onReject() {
    model->revertAll();
    this->close();
}
