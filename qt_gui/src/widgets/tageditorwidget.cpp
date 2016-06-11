#include "tageditorwidget.h"
#include "ui_tageditor.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>


TagEditorWidget::TagEditorWidget(QAbstractListModel* tagModel, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TagEditorWidget)
    , model{tagModel}
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->listViewTags->setModel(model);
    connect(ui->buttonBoxConfirm, SIGNAL(accepted()), this, SLOT(onAccept()));
    connect(ui->buttonBoxConfirm, SIGNAL(rejected()), this, SLOT(onReject()));
}

TagEditorWidget::~TagEditorWidget() { delete ui; }

void TagEditorWidget::onAccept() { this->close(); }

void TagEditorWidget::onReject() { this->close(); }
