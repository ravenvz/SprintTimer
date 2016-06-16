#ifndef TODOITEMVIEW_H
#define TODOITEMVIEW_H

#include "widgets/tageditorwidget.h"
#include "widgets/todoitemsviewdelegate.h"
#include <QDebug>
#include <QListView>


class TodoItemView : public QListView {
public:
    TodoItemView(QWidget* parent)
        : QListView(parent)
    {
        todoitemViewDelegate = new TodoItemsViewDelegate(this);
        setItemDelegate(todoitemViewDelegate);

        connect(this,
                &QListView::customContextMenuRequested,
                this,
                &TodoItemView::showTodoItemContextMenu);
    }

    // Overriding to catch dropEvent and prevent model start removing rows
    // attempting drag and drop by calling another method instead.
    void dropEvent(QDropEvent* event) override
    {
        int rowMovedFrom = currentIndex().row();
        int rowMovedTo = indexAt(event->pos()).row();
        model()->moveRows(
            QModelIndex(), rowMovedFrom, 1, QModelIndex(), rowMovedTo);
    }

    void setModels(TodoItemModel* taskModel, TagModel* tagModel)
    {
        setModel(taskModel);
        this->tagModel = tagModel;
    }

private:
    QPointer<TodoItemsViewDelegate> todoitemViewDelegate;
    TagModel* tagModel;
    QPointer<TagEditorWidget> tagEditor;

    void editTodoItem()
    {
        if (tagModel == nullptr) {
            qDebug() << "BOOM";
            return;
        }
        QModelIndex index = currentIndex();
        // TODO provide proper implementation of data() and setData() in
        // TodoItemModel, so that dynamic casts like that could be avoided
        TodoItem itemToEdit
            = dynamic_cast<TodoItemModel*>(model())->itemAt(index.row());
        AddTodoItemDialog dialog{tagModel};
        dialog.setWindowTitle("Edit TodoItem");
        dialog.fillItemData(itemToEdit);
        if (dialog.exec()) {
            TodoItem updatedItem = dialog.constructedTask();
            updatedItem.setSpentPomodoros(itemToEdit.spentPomodoros());
            updatedItem.setCompleted(itemToEdit.isCompleted());
            dynamic_cast<TodoItemModel*>(model())->replaceItemAt(index.row(),
                                                                 updatedItem);
        }
    }

    void removeTask()
    {
        QModelIndex index = currentIndex();
        ConfirmationDialog dialog;
        // TODO figure out a way to handle this situation more gracefully
        QString description;
        if (dynamic_cast<TodoItemModel*>(model())
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
            // If removing currently selected task, clear the linedit
            // if (selectedTask
            //     && todoitemViewModel->itemAt(index.row()).uuid()
            //         == selectedTask->uuid()) {
            //     ui->leDoneTask->clear();
            // }
            dynamic_cast<TodoItemModel*>(model())->remove(index);
        }
    }

    void launchTagEditor()
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

public slots:

    void showTodoItemContextMenu(const QPoint& pos)
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
};


#endif /* end of include guard: TODOITEMVIEW_H */
