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
#include "qt_gui/widgets/SimpleLegend.h"
#include <memory>
#include <ranges>

namespace sprint_timer::ui::qt_gui {

LegendItem::LegendItem(const QString& text_,
                       size_t itemIndex_,
                       QWidget* parent_)
    : QLabel{parent_}
    , itemIndex{itemIndex_}
{
    this->setText(text_);
}

void LegendItem::toggleSelected()
{
    selected = !selected;
    setStyleSheet(selected ? selectedStyle : normalStyle);
}

bool LegendItem::isSelected() const { return selected; }

void LegendItem::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked(itemIndex);
}

SimpleLegend::SimpleLegend(QWidget* parent_)
    : QWidget{parent_}
    , title{std::make_unique<QLabel>().release()}
{
    auto l = std::make_unique<QVBoxLayout>();
    l->addWidget(title);
    l->addStretch(1);
    setLayout(l.release());
}

void SimpleLegend::setData(std::span<const std::string> labels)
{
    for (auto* item : items) {
        layout()->removeWidget(item);
    }

    items.clear();

    qDeleteAll(findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));

    auto makeItem = [&, i = 0UL](const auto& label) mutable {
        auto item = std::make_unique<LegendItem>(
            QString::fromStdString(label), i++, this);
        layout()->addWidget(item.get());
        item->setVisible(true);
        connect(item.get(),
                &LegendItem::clicked,
                this,
                &SimpleLegend::onItemClicked);
        return item.release();
    };

    items.reserve(labels.size());
    std::ranges::transform(labels, begin(items), makeItem);
}

void SimpleLegend::setTitle(const QString& title_) { title->setText(title_); }

void SimpleLegend::toggleSelected(size_t index)
{
    items[index]->toggleSelected();
}

bool SimpleLegend::isSelected(size_t index) const
{
    return items[index]->isSelected();
}

void SimpleLegend::setTitleFont(QFont font) { title->setFont(font); }

void SimpleLegend::onItemClicked(size_t index) { emit itemClicked(index); }

} // namespace sprint_timer::ui::qt_gui

