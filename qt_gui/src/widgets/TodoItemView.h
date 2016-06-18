#ifndef TODOITEMVIEW_H
#define TODOITEMVIEW_H

#include "dialogs/AddTaskDialog.h"
#include "dialogs/confirmationdialog.h"
#include "models/TagModel.h"
#include "models/todoitemmodel.h"
#include "widgets/tageditorwidget.h"
#include "widgets/todoitemsviewdelegate.h"
#include <QDebug>
#include <QDropEvent>
#include <QListView>
#include <QMenu>
#include <QPointer>


class TodoItemView : public QListView {
public:
    explicit TodoItemView(QWidget* parent);

    // Overriding to catch dropEvent and prevent model start removing rows
    // attempting drag and drop by calling another method instead.
    void dropEvent(QDropEvent* event) final;

    void setModels(TaskModel* taskModel, TagModel* tagModel);

private:
    QPointer<TodoItemsViewDelegate> todoitemViewDelegate;
    TagModel* tagModel;
    QPointer<TagEditorWidget> tagEditor;

    void editTodoItem();

    void removeTask();

    void launchTagEditor();

public slots:

    void showTodoItemContextMenu(const QPoint& pos);
};


#endif /* end of include guard: TODOITEMVIEW_H */
