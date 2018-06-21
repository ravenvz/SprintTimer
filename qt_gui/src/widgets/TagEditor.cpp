/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "TagEditor.h"
#include "ui_tageditor.h"
#include <QMessageBox>

namespace sprint_timer::ui::qt_gui {


TagEditor::TagEditor(AsyncListModel* tagModel, QWidget* parent)
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

TagEditor::~TagEditor() { delete ui; }

} // namespace sprint_timer::ui::qt_gui

