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
#ifndef GAUGE_H
#define GAUGE_H

#include <QEvent>
#include <QWidget>
#include <iostream>
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
    Gauge(int actual, int goal, QWidget* parent);
    void setData(int actual, int goal);
    ~Gauge() = default;

private:
    int actual;
    int goal;
    std::unique_ptr<HoverState> hoveredState;
    std::unique_ptr<HoverState> unhoveredState;
    std::unique_ptr<WorkProgressState> workProgressUnderwork;
    std::unique_ptr<WorkProgressState> workProgressOverwork;
    std::unique_ptr<WorkProgressState> workProgressNone;
    std::unique_ptr<WorkProgressState> workProgressDone;
    HoverState* hoverState;
    WorkProgressState* workProgressState;
    bool sizesComputed = false;
    QRectF outerRect;
    QRectF innerRect;
    static QColor normalEmpty;
    static QColor normalFilled;
    static QColor overfilledEmpty;
    static QColor overfilledFilled;
    static QColor backgroundFree;
    static QColor backgroundHovered;

    void paintEvent(QPaintEvent*) override;
    bool eventFilter(QObject* object, QEvent* event) override;
    void computeAdaptiveSizes();
    void setupPainter(QPainter& painter);
    void drawOuterCircle(QPainter& painter);
    void drawInnerCircle(QPainter& painter);
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

class GaugeFactory {

public:
    GaugeFactory() {}

    Gauge* create(int filled, int total, QWidget* parent)
    {
        return new Gauge(filled, total, parent);
    }
};

#endif
