#ifndef ADDTODOITEMDIALOG_H
#define ADDTODOITEMDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QSqlTableModel>
#include "core/entities/TodoItem.h"

namespace Ui {
class AddTodoItemDialog;
}

class AddTodoItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTodoItemDialog(QSqlTableModel* tagModel, QWidget* parent = 0);
    ~AddTodoItemDialog();
    void accept() override;
    TodoItem getNewTodoItem();
    void fillItemData(TodoItem item);

private:
    Ui::AddTodoItemDialog* ui;
    TodoItem item;
    QSqlTableModel* tagModel;
    void setTagsModel();
    QStringList parseTags(QString& tagsString);

private slots:
    void quickAddTag(const QString& tag);
};

#endif // ADDTODOITEMDIALOG_H
