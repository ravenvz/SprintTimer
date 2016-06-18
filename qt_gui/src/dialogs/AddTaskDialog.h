#ifndef ADDTASKDIALOG_H_TBSYZEDZ
#define ADDTASKDIALOG_H_TBSYZEDZ


#include "core/entities/Task.h"
#include "models/TagModel.h"
#include <QDialog>
#include <QPointer>

namespace Ui {
class AddTaskDialog;
}

/* Provides front end to create or edit Task. */
class AddTaskDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddTaskDialog(TagModel* tagModel, QWidget* parent = nullptr);

    ~AddTaskDialog();

    /* Overidden to accept only if required fields are not empty.
     *
     * Empty required fields are marked with red border when trying to
     * accept dialog.*/
    void accept() override;

    /* Return Task constructed from data in UI elements. */
    Task constructedTask();

    /* Corresponding UI elements are filled with data from given Task. */
    void fillItemData(const Task& item);

private slots:
    /* Append tag to string of tags when selected from drop-down menu. */
    void onQuickAddTagActivated(const QString& tag);

    /* Remove red border (if present) when user starts to fill-out task name. */
    void resetNameLineEditStyle();

private:
    Ui::AddTaskDialog* ui;
    QPointer<TagModel> tagModel;
    QString requiredFieldEmptyStyle{"QLineEdit { border: 2px solid red; }"};

    void setTagsModel();
};

#endif /* end of include guard: ADDTASKDIALOG_H_TBSYZEDZ */
