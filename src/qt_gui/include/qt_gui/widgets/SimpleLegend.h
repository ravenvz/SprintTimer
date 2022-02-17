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
#ifndef SIMPLELEGEND_H_FM2CYP6M
#define SIMPLELEGEND_H_FM2CYP6M

#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <span>

#include <QDebug>

namespace sprint_timer::ui::qt_gui {

// TODO implement proper legend item.
/* Quick-and-dirty prototype for legend item. Needs some rework done.
 *
 * Designed to be used as an item in a layout of labels, thus
 * storing it's index.
 * Emits signal when clicked with left mouse button. */
class LegendItem : public QLabel {
    Q_OBJECT

public:
    LegendItem(const QString& text, size_t itemIndex, QWidget* parent);

    ~LegendItem() { qDebug() << "Legend item destroyed"; }

    /* Toggle state of the item. */
    void toggleSelected();

    /* Return true if item is in 'selected' state and false otherwise. */
    bool isSelected() const;

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    size_t itemIndex;
    QString selectedStyle{"QLabel { color: green; }"};
    QString normalStyle{"QLabel { color: black; }"};
    bool selected{false};

signals:
    /* Emitted when clicked with left mouse button. */
    void clicked(size_t itemIndex);
};

/* Displays labels in vertical layout with headline title.
 *
 * When clicked with left mouse button, emits itemClicked(size_t index) signal,
 * where index is the 0-based index of item in the list. */
class SimpleLegend : public QWidget {
    Q_OBJECT

public:
    explicit SimpleLegend(QWidget* parent);

    /* Set labels to display. */
    void setData(std::span<const std::string> labels);

    /* Overload that accepts vector of label-value pairs. Values are ignored. */

    /* Set headline title. */
    void setTitle(const QString& title);

    /* Mark item with given index as selected. */
    void toggleSelected(size_t index);

    /* Return true if item with given index is selected and false otherwise. */
    bool isSelected(size_t index) const;

    /* Set title font. */
    virtual void setTitleFont(QFont font);

private slots:
    /* Handle left mouse click. This implementation just emits
     * itemClicked(index). */
    void onItemClicked(size_t index);

private:
    // QVBoxLayout* layout;
    QLabel* title;
    std::vector<LegendItem*> items;

signals:
    void itemClicked(size_t index);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: SIMPLELEGEND_H_FM2CYP6M */
