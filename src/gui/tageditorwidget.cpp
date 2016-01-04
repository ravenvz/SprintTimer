#include "tageditorwidget.h"
#include "ui_tageditor.h"


TagEditorWidget::TagEditorWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TagEditorWidget)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    model = new QSqlTableModel();
    model->setTable("tag");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    ui->tableViewTags->setModel(model);
    ui->tableViewTags->hideColumn(0);
    ui->tableViewTags->resizeColumnToContents(1);
    connect(ui->buttonBoxConfirm, SIGNAL(accepted()), this, SLOT(onAccept()));
    connect(ui->buttonBoxConfirm, SIGNAL(rejected()), this, SLOT(onReject()));
}

TagEditorWidget::~TagEditorWidget() {
    delete model;
    delete ui;
}

void TagEditorWidget::onAccept() {
    model->submitAll();
    this->close();
}

void TagEditorWidget::onReject() {
    model->revertAll();
    this->close();
}
