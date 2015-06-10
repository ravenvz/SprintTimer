#include "todoitemsviewdelegate.h"
#include <QPainter>
#include <QApplication>

QSize TodoItemsViewDelegate::iconSize = QSize(60, 60);
int TodoItemsViewDelegate::padding = 5;

TodoItemsViewDelegate::TodoItemsViewDelegate(QObject* parent) :
    QStyledItemDelegate(parent)
{
}

TodoItemsViewDelegate::~TodoItemsViewDelegate() {
}

QSize TodoItemsViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
        return option.fontMetrics.boundingRect(option.rect, Qt::TextWordWrap, index.data().toString()).size();
    }

void TodoItemsViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
        painter->setPen(option.palette.highlightedText().color());
    } else {
        painter->setPen(option.palette.text().color());
    }
    painter->drawText(option.rect, Qt::TextWordWrap, index.data().toString());
}
