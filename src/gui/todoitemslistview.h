#ifndef TODOITEMSLISTVIEW_H
#define TODOITEMSLISTVIEW_H

#include <QListView>


class TodoItemsListView : public QListView
{
public:
    TodoItemsListView(QWidget* centralWidget);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);
};

#endif // TODOITEMSLISTVIEW_H
