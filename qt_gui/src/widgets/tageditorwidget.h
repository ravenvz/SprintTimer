#ifndef TAGEDITOR_H
#define TAGEDITOR_H

#include "models/AsyncListModel.h"
#include <QWidget>

namespace Ui {
class TagEditorWidget;
}

class TagEditorWidget : public QWidget {

    Q_OBJECT

public:
    TagEditorWidget(AsyncListModel* tagModel, QWidget* parent = nullptr);
    ~TagEditorWidget();

private:
    Ui::TagEditorWidget* ui;
    AsyncListModel* model;
};


#endif /* end of include guard: TAGEDITOR_H */
