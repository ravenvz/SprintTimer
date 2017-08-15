/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#ifndef GAUGE_H
#define GAUGE_H

#include <QEvent>
#include <QWidget>
#include <memory>


class HoverState;

class WorkProgressState;

class Gauge : public QWidget {
    Q_OBJECT

    friend class HoverState;
    friend class HoverStateHovered;
    friend class HoverStateUnhovered;
    friend class WorkProgressState;
    friend class WorkProgressNone;
    friend class WorkProgressDone;
    friend class WorkProgressUnderwork;
    friend class WorkProgressOverwork;

public:
    Gauge(int actual, int goal, double gaugeRelSize, QWidget* parent);
    void setData(int completed, int total);
    ~Gauge() = default;

private:
    int actual;
    int goal;
    const double gaugeRelSize;
    HoverState* hoverState;
    WorkProgressState* workProgressState;
    QRectF outerRect;
    QRectF innerRect;

    void paintEvent(QPaintEvent*) override;
    bool eventFilter(QObject* object, QEvent* event) override;
    void setupPainter(QPainter& painter);
    void drawOuterCircle(QPainter& painter);
    void drawInnerCircle(QPainter& painter);
    void updateState();
};


class HoverState {
public:
    virtual ~HoverState() = default;

    virtual void draw(const Gauge& gauge, QPainter& painter) = 0;

protected:
    virtual void
    drawText(const Gauge& gauge, QPainter& painter, const QString& text);
};

class HoverStateHovered : public HoverState {
public:
    void draw(const Gauge& gauge, QPainter& painter) final;
};

class HoverStateUnhovered : public HoverState {
public:
    void draw(const Gauge& gauge, QPainter& painter) final;
};

class WorkProgressState {
public:
    virtual void draw(const Gauge& gauge, QPainter& painter);

    virtual ~WorkProgressState() = default;

protected:
    QBrush empty;
    QBrush filled;

    virtual void setupBrushes();
};

class WorkProgressUnderwork : public WorkProgressState {
protected:
    void setupBrushes() final;
};

class WorkProgressOverwork : public WorkProgressState {
protected:
    void setupBrushes() final;
};

class WorkProgressDone : public WorkProgressState {
public:
    void draw(const Gauge& gauge, QPainter& painter) final;
};

class WorkProgressNone : public WorkProgressState {
public:
    void draw(const Gauge& gauge, QPainter& painter) final;
};

#endif
