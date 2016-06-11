#ifndef TAGEDITOR_H
#define TAGEDITOR_H

#include <QAbstractListModel>
#include <QWidget>

namespace Ui {
class TagEditorWidget;
}

class TagEditorWidget : public QWidget {

    Q_OBJECT

public:
    explicit TagEditorWidget(QAbstractListModel* tagModel, QWidget* parent = 0);
    ~TagEditorWidget();

private slots:
    void onAccept();
    void onReject();

private:
    Ui::TagEditorWidget* ui;
    QAbstractListModel* model;
};


#endif /* end of include guard: TAGEDITOR_H */
