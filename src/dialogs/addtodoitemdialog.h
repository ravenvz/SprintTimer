#ifndef ADDTODOITEMDIALOG_H
#define ADDTODOITEMDIALOG_H

#include <QDialog>
#include <QPointer>
#include "core/entities/TodoItem.h"
#include "models/tagmodel.h"

namespace Ui {
class AddTodoItemDialog;
}

class AddTodoItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTodoItemDialog(TagModel* tagModel, QWidget* parent = 0);
    ~AddTodoItemDialog();
    void accept() override;
    TodoItem getNewTodoItem();
    void fillItemData(TodoItem item);

private:
    Ui::AddTodoItemDialog* ui;
    TodoItem item;
    QPointer<TagModel> tagModel;
    void setTagsModel();
    QStringList parseTags(QString& tagsString);

private slots:
    void quickAddTag(const QString& tag);
    void resetNameLineEditStyle();
};

#endif // ADDTODOITEMDIALOG_H
