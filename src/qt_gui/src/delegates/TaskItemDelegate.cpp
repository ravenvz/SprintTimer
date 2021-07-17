/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "qt_gui/delegates/TaskItemDelegate.h"
#include "qt_gui/metatypes/TaskDTOMetatype.h"
#include "qt_gui/models/CustomRoles.h"
#include "core/use_cases/TaskDTO.h"
#include <QApplication>
#include <QPainter>
#include "core/utils/StringUtils.h"

namespace {

// Note that below options must match those defined in QSS.
// Probably there is a cleaner way to do this.
constexpr int margin{5};
constexpr int pad{5};
constexpr int borderWidth{1};

constexpr int offset{margin + borderWidth + pad};

constexpr int completedItemAlpha{100};
constexpr int normalItemAlpha{255};

std::tuple<QRect, QRect, QRect>
textRectangles(const QStyleOptionViewItem& option, const QModelIndex& index);

QRect boundingRect(const QStyleOptionViewItem& option,
                   const QString& text,
                   int maxWidth,
                   int flags);

int contentWidth(const QRect& rect);

std::tuple<QString, QString, QString>
extractData(const sprint_timer::use_cases::TaskDTO& item);

} // namespace

namespace sprint_timer::ui::qt_gui {

TaskItemDelegate::TaskItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
    delimiterColor.setAlpha(50);
}

QSize TaskItemDelegate::sizeHint(const QStyleOptionViewItem& option,
                                 const QModelIndex& index) const
{
    if (!index.isValid())
        return QSize();

    const auto [statsRect, tagsRect, descrRect] = textRectangles(option, index);
    return QSize{option.rect.width(),
                 tagsRect.height() + descrRect.height() + 3 * offset};
}

void TaskItemDelegate::paint(QPainter* painter,
                             const QStyleOptionViewItem& option,
                             const QModelIndex& index) const
{

    QStyleOptionViewItem opt{option};
    initStyleOption(&opt, index);
    QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();

    painter->save();
    if (index.data(Qt::CheckStateRole).toBool()) {
        QFont strikedOutFont{opt.font};
        strikedOutFont.setStrikeOut(true);
        painter->setFont(strikedOutFont);
        paintItem(painter, opt, index, completedItemAlpha);
    }
    else {
        paintItem(painter, opt, index, normalItemAlpha);
    }

    opt.text = "";
    opt.features ^= QStyleOptionViewItem::HasCheckIndicator;
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
    painter->restore();
}

void TaskItemDelegate::paintItem(QPainter* painter,
                                 const QStyleOptionViewItem& option,
                                 const QModelIndex& index,
                                 int alpha) const
{
    auto taskVariant = index.data(CustomRoles::ItemRole);
    if (taskVariant.isNull()) {
        return;
    }

    QColor tagCol{tagColor};
    QColor commonCol{commonColor};
    QColor overspentCol{overspentColor};

    tagCol.setAlpha(alpha);
    commonCol.setAlpha(alpha);
    overspentCol.setAlpha(alpha);

    const auto item = taskVariant.value<use_cases::TaskDTO>();
    const auto [tags, descr, completionStats] = extractData(item);

    auto [statsRect, tagsRect, descrRect] = textRectangles(option, index);

    const auto contentRect{option.rect};
    QStyle* style =
        option.widget ? option.widget->style() : QApplication::style();

    // Truncate rect for a very long tag, that doesn't fit into rect
    tagsRect.setWidth(contentWidth(contentRect) - statsRect.width() - offset);
    tagsRect.adjust(contentRect.topLeft().x() + offset,
                    contentRect.topLeft().y() + offset,
                    contentRect.topLeft().x() + offset,
                    contentRect.topLeft().y() + offset);
    painter->setPen(tagCol);
    style->drawItemText(
        painter, tagsRect, Qt::TextWordWrap, option.palette, true, tags);

    // Truncate rect for a very long word, that doesn't fit into rect
    descrRect.setWidth(std::min(descrRect.width(), contentWidth(contentRect)));
    descrRect.adjust(contentRect.bottomLeft().x() + offset,
                     contentRect.bottomLeft().y() - offset - descrRect.height(),
                     contentRect.bottomLeft().x() + offset,
                     contentRect.bottomLeft().y() - offset -
                         descrRect.height());
    painter->setPen(commonCol);
    style->drawItemText(
        painter, descrRect, Qt::TextWordWrap, option.palette, true, descr);

    statsRect.adjust(contentRect.topRight().x() - offset - statsRect.width(),
                     contentRect.topRight().y() + offset,
                     contentRect.topRight().x() - offset - statsRect.width(),
                     contentRect.topRight().y() + offset);
    if (item.actualCost > item.expectedCost) {
        painter->setPen(overspentCol);
    }
    style->drawItemText(painter,
                        statsRect,
                        Qt::AlignRight | Qt::AlignBottom,
                        option.palette,
                        true,
                        completionStats);
}

} // namespace sprint_timer::ui::qt_gui

namespace {

std::tuple<QRect, QRect, QRect>
textRectangles(const QStyleOptionViewItem& option, const QModelIndex& index)
{
    using sprint_timer::use_cases::TaskDTO;
    using sprint_timer::ui::qt_gui::CustomRoles;
    const auto taskVariant = index.data(CustomRoles::ItemRole);
    const auto item = taskVariant.value<TaskDTO>();
    const auto [tags, descr, stats] = extractData(item);

    QRect statsRect{boundingRect(option, stats, contentWidth(option.rect), 0)};
    QRect tagsRect{
        boundingRect(option,
                     tags,
                     contentWidth(option.rect) - statsRect.width() - offset,
                     Qt::TextWordWrap)};
    QRect descrRect{boundingRect(
        option, descr, contentWidth(option.rect), Qt::TextWordWrap)};

    return {statsRect, tagsRect, descrRect};
}

QRect boundingRect(const QStyleOptionViewItem& option,
                   const QString& text,
                   int maxWidth,
                   int flags)
{
    QFontMetrics metrics{option.font};
    return QRect{metrics.boundingRect(0, 0, maxWidth, 0, flags, text)};
}

int contentWidth(const QRect& rect) { return rect.width() - 2 * offset; }

std::tuple<QString, QString, QString>
extractData(const sprint_timer::use_cases::TaskDTO& item)
{
    using namespace sprint_timer::utils;
    const QString tags{QString::fromStdString(
        transformJoin(item.tags, " ", [](const auto& elem) {
            std::string res{"#"};
            res += elem;
            return res;
        }))};
    return {tags,
            QString::fromStdString(item.name),
            QString("%1/%2").arg(item.actualCost).arg(item.expectedCost)};
}

} // namespace
