#ifndef ADDTODOITEMDIALOG_H
#define ADDTODOITEMDIALOG_H

#include <QDialog>
#include "core/entities.h"
#include <QStringListModel>

namespace Ui {
class AddTodoItemDialog;
}

class AddTodoItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTodoItemDialog(QWidget* parent = 0);
    ~AddTodoItemDialog();
    void accept() override;
    TodoItem getNewTodoItem();
    void fillItemData(TodoItem item);

private:
    Ui::AddTodoItemDialog *ui;
    TodoItem item;
    QStringListModel* tagModel;
    static const int tempId = -1;
    void setTagsModel();

private slots:
    void quickAddTag(const QString& tag);
};

#endif // ADDTODOITEMDIALOG_H
