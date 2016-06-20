#include "tageditorwidget.h"
#include "ui_tageditor.h"
#include <QMessageBox>


TagEditorWidget::TagEditorWidget(AsyncListModel* tagModel, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TagEditorWidget)
    , model{tagModel}
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->listViewTags->setModel(model);
    connect(ui->buttonBoxConfirm,
            &QDialogButtonBox::accepted,
            this,
            &QWidget::close);
    connect(ui->buttonBoxConfirm,
            &QDialogButtonBox::rejected,
            this,
            &QWidget::close);
    connect(ui->buttonBoxConfirm,
            &QDialogButtonBox::accepted,
            model,
            &AsyncListModel::submitData);
    connect(ui->buttonBoxConfirm,
            &QDialogButtonBox::rejected,
            model,
            &AsyncListModel::revertData);
}

TagEditorWidget::~TagEditorWidget() { delete ui; }
