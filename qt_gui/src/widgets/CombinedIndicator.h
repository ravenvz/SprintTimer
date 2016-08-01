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

#ifndef COMBINEDINDICATOR_H_4ZPMFI3J
#define COMBINEDINDICATOR_H_4ZPMFI3J

#include "TimerWidgetBase.h"
#include <QMouseEvent>
#include <QWidget>

/* Widget that is capable of drawing an arc as an indicator of
 * some progress along with some short status text in the middle. */
class CombinedIndicator : public QWidget {

    Q_OBJECT

public:
    /* Construct indicator. Takes sideSizeHint as a hint to indicator
     * width and height. */
    CombinedIndicator(int sideSizeHint, QWidget* parent);

    ~CombinedIndicator() = default;

    QSize sizeHint() const override;

    /* Set value that corresponds to a maximum value that full arc (empty arc
     * in the inverted mode) should represent. */
    void setMaxValue(int value);

    /* Set value that widget should display relative to maximum value. */
    void setCurrentValue(int value);

    /* Set text that should be displayed in the middle of the widget.
     *
     * Text length is ment to be short enough to fit inside, as no scaling of
     * text size relative to it's length is currently implemented. Although,
     * font size will be scaled to widget's size. */
    void setText(QString&& text);

    /* Set color of arc. */
    void setColor(const QColor& color);

    /* Set drawing style.
     *
     * When normal style is set, the minimum value is represented by an arc
     * with 0 degrees span angle, and span angle will increase when value is
     * increased.
     *
     * When inverted style is set, the minimum value is represented by an arc
     * with 360 degrees span angle, and span angle will decrease when value is
     * increased. */
    void setInvertedStyle(bool inverted);

    void setDrawArc(bool shouldDraw);

signals:
    /* emitted when widget is clicked with left mouse button. */
    void indicatorClicked();

private:
    int widthAndHeightHint;
    bool drawArc{false};
    int currentValue{0};
    int maxValue{0};
    bool invertedDrawStyle{false};
    QString currentText;
    QColor color;

    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
    void drawProgressArc(QPainter& painter) const;
    void drawText(QPainter& painter) const;
};

#endif /* end of include guard: COMBINEDINDICATOR_H_4ZPMFI3J */
