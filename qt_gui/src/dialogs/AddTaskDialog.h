#ifndef ADDTODOITEMDIALOG_H
#define ADDTODOITEMDIALOG_H

#include "core/entities/TodoItem.h"
#include "models/TagModel.h"
#include <QDialog>
#include <QPointer>

namespace Ui {
class AddTodoItemDialog;
}

/* Provides front end to create or edit Task. */
class AddTodoItemDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddTodoItemDialog(TagModel* tagModel, QWidget* parent = 0);

    ~AddTodoItemDialog();

    /* Overidden to accept only if required fields are not empty.
     *
     * Empty required fields are marked with red border when trying to
     * accept dialog.*/
    void accept() override;

    /* Return Task constructed from data in UI elements. */
    TodoItem constructedTask();

    /* Corresponding UI elements are filled with data from given Task. */
    void fillItemData(const TodoItem& item);

private slots:
    /* Append tag to string of tags when selected from drop-down menu. */
    void onQuickAddTagActivated(const QString& tag);

    /* Remove red border (if present) when user starts to fill-out task name. */
    void resetNameLineEditStyle();

private:
    Ui::AddTodoItemDialog* ui;
    QPointer<TagModel> tagModel;
    QString requiredFieldEmptyStyle{"QLineEdit { border: 2px solid red; }"};

    void setTagsModel();
};

#endif // ADDTODOITEMDIALOG_H
