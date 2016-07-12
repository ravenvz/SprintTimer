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

class CombinedIndicator : public QWidget {

    Q_OBJECT

public:
    CombinedIndicator(QWidget* parent = nullptr);
    ~CombinedIndicator() = default;
    QSize sizeHint() const override;
    void updateIndication(Second timeLeft);
    void setDuration(Second duration);
    void setStatus(QString&& status);
    void setColor(const QColor& color);

private:
    bool running{false};
    Second timeLeft{0};
    Second timerDuration{0};
    QString status;
    QColor color;

    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
    void drawProgress(QPainter& painter) const;
    void drawProgressBar(QPainter& painter) const;
    void drawStatusText(QPainter& painter) const;

signals:
    void indicatorClicked();
};

#endif /* end of include guard: COMBINEDINDICATOR_H_4ZPMFI3J */
