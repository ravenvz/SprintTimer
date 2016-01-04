#ifndef TAGEDITOR_H
#define TAGEDITOR_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
    class TagEditorWidget;
}

class TagEditorWidget : public QWidget {

    Q_OBJECT

public:
    explicit TagEditorWidget(QWidget* parent = 0);
    ~TagEditorWidget();

private slots:
    void onAccept();
    void onReject();

signals:
    void dataSetChanged();

private:
    Ui::TagEditorWidget* ui;
    QSqlTableModel* model;


};



#endif /* end of include guard: TAGEDITOR_H */
