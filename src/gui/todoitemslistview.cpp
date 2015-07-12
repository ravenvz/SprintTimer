#include "todoitemslistview.h"
#include <iostream>

TodoItemsListView::TodoItemsListView(QWidget* centralWidget)
    : QListView(centralWidget)
{
}

// void TodoItemsListView::dragEnterEvent(QDragEnterEvent* event) {
//     std::cout << "Enter Drag Event" << std::endl;
// //    event.acceptProposedAction();
// }
//
// void TodoItemsListView::dragMoveEvent(QDragMoveEvent* event) {
//
// }
//
// void TodoItemsListView::dropEvent(QDropEvent* event) {
//
// }
