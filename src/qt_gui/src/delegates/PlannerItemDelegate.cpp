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
#include "qt_gui/delegates/PlannerItemDelegate.h"
#include "api/dtos/TaskTreeDTO.h"
#include "qt_gui/models/CustomRoles.h"
#include "qt_gui/presentation/PlannerContract.h"
#include <QApplication>
#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItem>

#include <QDebug>

namespace {

constexpr int checkerOffset{25};

struct PainterGuard {
    explicit PainterGuard(QPainter* painter_)
        : painter{painter_}
    {
        painter->save();
    }

    ~PainterGuard() { painter->restore(); }

private:
    QPainter* painter;
};

class ColorScheme {
public:
    virtual ~ColorScheme() = default;

    [[nodiscard]] virtual auto tagColor() const -> QColor = 0;

    [[nodiscard]] virtual auto commonColor() const -> QColor = 0;

    [[nodiscard]] virtual auto overWorkColor() const -> QColor = 0;

    [[nodiscard]] virtual auto delimiterColor() const -> QColor = 0;
};

class ColorSchemeQt : public ColorScheme {
public:
    [[nodiscard]] auto commonColor() const -> QColor override { return common; }

    [[nodiscard]] auto tagColor() const -> QColor override { return tag; }

    [[nodiscard]] auto overWorkColor() const -> QColor override
    {
        return overspent;
    }

    [[nodiscard]] auto delimiterColor() const -> QColor override
    {
        return delimiter;
    }

private:
    QColor tag{Qt::blue};
    QColor common{Qt::black};
    QColor overspent{Qt::red};
    QColor delimiter{Qt::gray};
};

} // namespace

namespace sprint_timer::ui::qt_gui {

PlannerItemDelegate::PlannerItemDelegate(QObject* parent_)
    : QStyledItemDelegate{parent_}
{
}

// void PlannerItemDelegate::initStyleOption(QStyleOptionViewItem* option,
//                                           const QModelIndex& index) const
// {
//     if (index.column() == 0) {
//         QStyledItemDelegate::initStyleOption(option, index);
//         option->decorationAlignment = Qt::AlignLeft;
//         option->displayAlignment = Qt::AlignLeft;
//     }
//     if (index.column() == 2) {
//         QStyledItemDelegate::initStyleOption(option, index);
//         option->textElideMode = Qt::ElideMiddle;
//         option->displayAlignment = Qt::AlignHCenter;
//     }
//     // QStyledItemDelegate::initStyleOption(option, index);
//     qDebug() << "displayAlignment: " << option->displayAlignment;
//     qDebug() << "decorationAlignment: " << option->decorationAlignment;
//     // qDebug() << "textElide: " << option->textElideMode;
// }

auto PlannerItemDelegate::sizeHint(const QStyleOptionViewItem& option,
                                   const QModelIndex& index) const -> QSize
{
    if (!index.isValid()) {
        return {};
    }

    const auto data = index.data(Qt::DisplayRole);
    const auto [payload, pen, brush] =
        data.value<std::tuple<QString, QColor, QColor>>();

    // if (index.column() == 0) {
    //     return {};
    // }
    if (index.column() == 1) {
        return {};
    }

    QStyleOptionViewItem opt{option};
    initStyleOption(&opt, index);
    QFontMetrics metrics{option.font};

    const auto rect =
        metrics.boundingRect(0, 0, opt.rect.width(), 0, 0, payload);
    if (index.column() == 0) {
        return QSize{rect.width() + checkerOffset, rect.height()};
    }
    return QSize{rect.width() + 5, rect.height()};
}

auto PlannerItemDelegate::paint(QPainter* painter,
                                const QStyleOptionViewItem& option,
                                const QModelIndex& index) const -> void
{
    using contracts::PlannerContract::Colors;
    using contracts::PlannerContract::PlannerItem;

    if (index.column() > 4) {
        return;
    }

    QStyleOptionViewItem opt{option};
    initStyleOption(&opt, index);

    QStyle* style =
        opt.widget != nullptr ? opt.widget->style() : QApplication::style();

    ColorSchemeQt colorScheme;

    PainterGuard painterGuard{painter};

    if (index.data(Qt::CheckStateRole) == Qt::Checked) {
        QFont strikedOutFont{opt.font};
        strikedOutFont.setStrikeOut(true);
        painter->setFont(strikedOutFont);
    }

    auto paintDefaultDataTuple = [&](const auto& rect) {
        const auto data = index.data(Qt::DisplayRole);
        if (not data.isNull()) {
            const auto [payload, foreground, background] =
                data.value<std::tuple<QString, QColor, QColor>>();
            painter->setPen(background);
            painter->setBrush(background);
            painter->drawRect(opt.rect);
            painter->setPen(foreground);

            style->drawItemText(
                painter, rect, Qt::TextDontClip, opt.palette, true, payload);
        }
    };

    // Displaying checkbox
    if (index.column() == 0) {
        const auto data =
            index.model()
                ->data(index.model()->index(index.row(), 5, index.parent()),
                       Qt::DisplayRole)
                .value<std::tuple<std::string, bool, api::TaskTypeDTO>>();
        paintDefaultDataTuple(opt.rect.translated(checkerOffset, 0));
        if (std::get<2>(data) == api::TaskTypeDTO::Folder) {
            return;
        }
        QStyledItemDelegate::paint(painter, opt, index);
        return;
    }

    paintDefaultDataTuple(opt.rect);
}

} // namespace sprint_timer::ui::qt_gui

