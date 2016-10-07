/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include <QPainter>
#include <QApplication>
#include "TaskViewDelegate.h"


QRect constructBoundingRect(const QStyleOptionViewItem& option, const QString& text, const int flags);

void rescaleTagsRectIfMultiline(QRect& tagsRect, const QRect& statRect, const QRect& optionRect);

bool taskOverspent(const QString& stats);

TaskViewDelegate::TaskViewDelegate(QObject* parent) :
    QStyledItemDelegate(parent)
{
    delimiterColor.setAlpha(50);
}

QSize TaskViewDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (!index.isValid()) return QSize();

    QString tags = index.data(Qt::UserRole + 1).toString();
    QString name = index.data(Qt::UserRole + 2).toString();
    QString completionStats = index.data(Qt::UserRole + 3).toString();

    QRect tagsRect {constructBoundingRect(option, tags, Qt::AlignLeft)};
    QRect statRect {constructBoundingRect(option, completionStats, Qt::AlignRight)};
    QRect nameRect {constructBoundingRect(option, name, Qt::AlignLeft | Qt::TextWordWrap)};

    rescaleTagsRectIfMultiline(tagsRect, statRect, option.rect);

    return QSize {option.rect.width(), tagsRect.height() + nameRect.height() + 3 * padding + verticalSpacing};
}

QRect constructBoundingRect(const QStyleOptionViewItem& option, const QString& text, const int flags) {
    QFontMetrics metrics {option.font};
    return QRect {metrics.boundingRect(0, 0, option.rect.width(), 0, flags, text)};
}

void TaskViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();
    QFont completedItemFont {option.font};
    painter->setFont(completedItemFont);
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
        painter->setPen(option.palette.highlightedText().color());
    }
    if (index.data(Qt::CheckStateRole).toBool()) {
        completedItemFont.setStrikeOut(true);
        painter->setFont(completedItemFont);
        paintText(painter, option, index, completedItemAlpha);
    } else {
        paintText(painter, option, index, normalItemAlpha);
    }
    painter->restore();
}

void TaskViewDelegate::paintText(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, const int alpha) const {
    QColor tagCol = tagColor;
    QColor commonCol = commonColor;
    QColor overspentCol = overspentColor;
    tagCol.setAlpha(alpha);
    commonCol.setAlpha(alpha);
    overspentCol.setAlpha(alpha);

    QString tags = index.data(Qt::UserRole + 1).toString();
    QString descr = index.data(Qt::UserRole + 2).toString();
    QString completionStats = index.data(Qt::UserRole + 3).toString();

    // Stats should be drawn in the bottom-right corner of the line where tags are drawn
    // So we set tagsRect width to match item width and add corrections in height.
    QRect tagsRect {constructBoundingRect(option, tags, Qt::AlignLeft | Qt::TextWordWrap)};
    tagsRect.setWidth(option.rect.width());
    tagsRect.adjust(option.rect.topLeft().x(), option.rect.topLeft().y() + verticalSpacing,
                    0, option.rect.topLeft().y() + verticalSpacing);

    painter->setPen(tagCol);
    painter->drawText(tagsRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, tags);

    painter->setPen(commonCol);
    painter->drawText(option.rect, Qt::AlignLeft | Qt::TextWordWrap | Qt::AlignBottom, descr);

    if (taskOverspent(completionStats)) {
        painter->setPen(overspentCol);
    }
    painter->drawText(tagsRect, Qt::AlignRight | Qt::AlignBottom, completionStats);

    QPoint lineBegin {tagsRect.topLeft().x() + 2 * padding,
                      tagsRect.topLeft().y() - verticalSpacing / 2};
    QPoint lineEnd {tagsRect.topLeft().x() + tagsRect.width() - 1 * padding, 
                    tagsRect.topLeft().y() - verticalSpacing / 2};
    if (index.row() != 0) {
        painter->setPen(delimiterColor);
        painter->drawLine(lineBegin, lineEnd);
    }
}

void rescaleTagsRectIfMultiline(QRect& tagsRect, const QRect& statRect, const QRect& optionRect) {
    if (optionRect.width() > 0 && (tagsRect.width() + statRect.width()) > optionRect.width()) {

        int scaleFactor = (tagsRect.width() + statRect.width()) / optionRect.width() + 1;
        tagsRect.setHeight(scaleFactor * tagsRect.height() + scaleFactor * TaskViewDelegate::padding);
    }
}

bool taskOverspent(const QString& stats) {
    QStringList parts = stats.split("/");
    return (parts[0].toInt() > parts[1].toInt()); 
}

