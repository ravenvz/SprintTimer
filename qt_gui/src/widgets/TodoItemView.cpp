#include "TodoItemView.h"

TodoItemView::TodoItemView(QWidget* parent)
    : QListView(parent)
{
    todoitemViewDelegate = new TodoItemsViewDelegate(this);
    setItemDelegate(todoitemViewDelegate);

    connect(this,
            &QListView::customContextMenuRequested,
            this,
            &TodoItemView::showTodoItemContextMenu);
}

void TodoItemView::dropEvent(QDropEvent* event)
{
    int rowMovedFrom = currentIndex().row();
    int rowMovedTo = indexAt(event->pos()).row();
    model()->moveRows(
        QModelIndex(), rowMovedFrom, 1, QModelIndex(), rowMovedTo);
}

void TodoItemView::setModels(TaskModel* taskModel, TagModel* tagModel)
{
    setModel(taskModel);
    this->tagModel = tagModel;
}

void TodoItemView::editTodoItem()
{
    if (tagModel == nullptr) {
        // TODO log error
        qDebug() << "Passing null tagModel to TodoItemView";
        return;
    }
    QModelIndex index = currentIndex();
    // TODO provide proper implementation of data() and setData() in
    // TaskModel, so that dynamic casts like that could be avoided
    Task itemToEdit
        = dynamic_cast<TaskModel*>(model())->itemAt(index.row());
    AddTodoItemDialog dialog{tagModel};
    dialog.setWindowTitle("Edit Task");
    dialog.fillItemData(itemToEdit);
    if (dialog.exec()) {
        Task updatedItem = dialog.constructedTask();
        updatedItem.setSpentPomodoros(itemToEdit.spentPomodoros());
        updatedItem.setCompleted(itemToEdit.isCompleted());
        dynamic_cast<TaskModel*>(model())->replaceItemAt(index.row(),
                                                             updatedItem);
    }
}

void TodoItemView::removeTask()
{
    QModelIndex index = currentIndex();
    ConfirmationDialog dialog;
    // TODO figure out a way to handle this situation more gracefully
    QString description;
    if (dynamic_cast<TaskModel*>(model())
            ->itemAt(index.row())
            .spentPomodoros()
        > 0) {
        description
            = "WARNING! This todo item has pomodoros associated with it "
              "and they will be removed permanently along with this item.";
    }
    else {
        description = "This will delete todo item permanently!";
    }
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        dynamic_cast<TaskModel*>(model())->remove(index);
    }
}

void TodoItemView::launchTagEditor()
{
    if (!tagEditor) {
        tagEditor = new TagEditorWidget{tagModel};
        tagEditor->show();
    }
    else {
        tagEditor->raise();
        tagEditor->activateWindow();
        tagEditor->showNormal();
    }
}

void TodoItemView::showTodoItemContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu todoItemsMenu;
    // Note QMenu takes ownership of Action
    todoItemsMenu.addAction("Edit");
    todoItemsMenu.addAction("Delete");
    todoItemsMenu.addAction("Tag editor");

    QAction* selectedItem = todoItemsMenu.exec(globalPos);

    if (selectedItem && selectedItem->text() == "Edit")
        editTodoItem();
    if (selectedItem && selectedItem->text() == "Delete")
        removeTask();
    if (selectedItem && selectedItem->text() == "Tag editor")
        launchTagEditor();
}
