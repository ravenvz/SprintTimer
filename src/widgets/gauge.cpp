#include "gauge.h"
#include <QPainter>
#include <QWidget>

#include <QDebug>


Gauge::Gauge(int actual, int goal, QWidget* parent)
    : QWidget(parent)
    , hoveredState{std::make_unique<HoverStateHovered>()}
    , unhoveredState{std::make_unique<HoverStateUnhovered>()}
    , workProgressUnderwork{std::make_unique<WorkProgressUnderwork>()}
    , workProgressOverwork{std::make_unique<WorkProgressOverwork>()}
    , workProgressNone{std::make_unique<WorkProgressNone>()}
    , workProgressDone{std::make_unique<WorkProgressDone>()}
    , hoverState{unhoveredState.get()}

{
    installEventFilter(this);
    setMouseTracking(true);
    setData(actual, goal);
}

void Gauge::setData(int actual, int goal)
{
    this->actual = actual;
    this->goal = goal;

    if (actual == 0 || goal == 0) {
        workProgressState = workProgressNone.get();
    }
    else if (actual == goal) {
        workProgressState = workProgressDone.get();
    }
    else if (actual > goal) {
        workProgressState = workProgressOverwork.get();
    }
    else {
        workProgressState = workProgressUnderwork.get();
    }
}

void Gauge::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    setupPainter(painter);
    drawOuterCircle(painter);
    drawInnerCircle(painter);
}

void Gauge::setupPainter(QPainter& painter)
{
    if (!sizesComputed)
        computeAdaptiveSizes();
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidthF(1.2);
    painter.setPen(pen);
}

void Gauge::computeAdaptiveSizes()
{
    QRectF totalSizeRect = this->rect();
    double outerRectLength
        = 0.8 * std::min(totalSizeRect.width(), totalSizeRect.height());
    double innerRectLength = 0.7 * outerRectLength;
    outerRect = QRectF(totalSizeRect.center().x() - outerRectLength / 2,
                       totalSizeRect.center().y() - outerRectLength / 2,
                       outerRectLength,
                       outerRectLength);
    innerRect = QRectF(totalSizeRect.center().x() - innerRectLength / 2,
                       totalSizeRect.center().y() - innerRectLength / 2,
                       innerRectLength,
                       innerRectLength);
    sizesComputed = true;
}

void Gauge::drawOuterCircle(QPainter& painter)
{
    workProgressState->draw(painter, outerRect, actual, goal);
}

void Gauge::drawInnerCircle(QPainter& painter)
{
    hoverState->draw(painter, innerRect, actual, goal);
}

bool Gauge::eventFilter(QObject* object, QEvent* event)
{
    if (object != this)
        return false;
    if (event->type() == QEvent::Enter) {
        setFocus();
        hoverState = hoveredState.get();
        this->repaint();
        return true;
    }
    if (event->type() == QEvent::Leave) {
        hoverState = unhoveredState.get();
        this->repaint();
        return true;
    }
    return false;
}

void HoverState::drawText(QPainter& painter,
                          const QRectF& rect,
                          const QString& text)
{
    QFont font = painter.font();
    font.setPixelSize(static_cast<int>(0.3 * rect.width()));
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, text);
}

void HoverStateHovered::draw(QPainter& painter,
                             const QRectF& rect,
                             int actual,
                             int goal)
{
    QString text = QString("%1").arg(actual);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);
    painter.drawEllipse(rect);
    HoverState::drawText(painter, rect, text);
}

void HoverStateUnhovered::draw(QPainter& painter,
                               const QRectF& rect,
                               int actual,
                               int goal)
{
    QString text = QString("%1%").arg(goal != 0 ? actual * 100 / goal : 0);
    painter.setBrush(QColor("#354a5f"));
    painter.drawEllipse(rect);
    painter.setPen(Qt::white);
    HoverState::drawText(painter, rect, text);
}

void WorkProgressState::draw(QPainter& painter,
                             const QRectF& rect,
                             int actual,
                             int goal,
                             const QBrush& empty,
                             const QBrush& filled)
{
    const int fullCircle = 360 * 16;
    const int offsetToTop = 90 * 16;
    int completedAngle
        = static_cast<int>((actual % goal) * fullCircle / float(goal));
    painter.setBrush(empty);
    painter.drawEllipse(rect);
    painter.setBrush(filled);
    painter.drawPie(rect, offsetToTop, -completedAngle);
}

void WorkProgressUnderwork::draw(QPainter& painter,
                                 const QRectF& rect,
                                 int actual,
                                 int goal)
{
    WorkProgressState::draw(painter,
                            rect,
                            actual,
                            goal,
                            QBrush{Qt::gray},
                            QBrush{QColor{"#6baa15"}});
}

void WorkProgressOverwork::draw(QPainter& painter,
                                const QRectF& rect,
                                int actual,
                                int goal)
{
    WorkProgressState::draw(painter,
                            rect,
                            actual,
                            goal,
                            QBrush{QColor{"#6baa15"}},
                            QBrush{Qt::red});
}

void WorkProgressDone::draw(QPainter& painter,
                            const QRectF& rect,
                            int actual,
                            int goal)
{
    // QBrush brush{actual == 0 ? Qt::gray : QColor{"#6baa15"}};
    painter.setBrush(QColor{"#6baa15"});
    painter.drawEllipse(rect);
}

void WorkProgressNone::draw(QPainter& painter,
                            const QRectF& rect,
                            int actual,
                            int goal)
{
    painter.setBrush(Qt::gray);
    painter.drawEllipse(rect);
}
