/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "widgets/SimpleLegend.h"
#include <iostream>

LegendItem::LegendItem(const QString& text, size_t itemIndex, QWidget* parent)
    : QLabel(parent)
    , itemIndex(itemIndex)
{
    this->setText(text);
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

SimpleLegend::SimpleLegend(QWidget* parent)
    : IStatisticalChartLegend(parent)
{
    layout = new QVBoxLayout();
    title = new QLabel();
    layout->addWidget(title);
    layout->addStretch(1);
    setLayout(layout);
}

void SimpleLegend::setData(
    const std::vector<std::pair<std::string, double>>& data)
{
    std::vector<std::string> labels;
    std::transform(data.cbegin(),
                   data.cend(),
                   std::back_inserter(labels),
                   [](const auto& elem) { return elem.first; });
    setData(labels);
}

void SimpleLegend::setData(const std::vector<std::string>& labels)
{
    for (auto item : items) {
        layout->removeWidget(item);
        delete item;
    }
    items.clear();
    for (size_t i = 0; i < labels.size(); ++i) {
        LegendItem* item = new LegendItem(
            QString::fromStdString(labels[i]), i, this);
        layout->addWidget(item);
        item->setVisible(true);
        connect(
            item, SIGNAL(clicked(size_t)), this, SLOT(onItemClicked(size_t)));
        items.push_back(item);
    }
}

void SimpleLegend::setTitle(const QString& title)
{
    this->title->setText(title);
}

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
