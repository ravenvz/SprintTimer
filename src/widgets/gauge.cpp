#include "gauge.h"
#include <QPainter>
#include <QWidget>

#include <QDebug>

// static
QColor Gauge::normalEmpty{Qt::gray};
QColor Gauge::normalFilled{QColor{"#6baa15"}};
QColor Gauge::overfilledEmpty{QColor{"#6baa15"}};
QColor Gauge::overfilledFilled{Qt::red};
QColor Gauge::backgroundFree{QColor{"#354a5f"}};
QColor Gauge::backgroundHovered{Qt::white};

Gauge::Gauge(int actual, int goal, QWidget* parent)
    : QWidget(parent)
    , actual{actual}
    , goal{goal}
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
    workProgressState->draw(*this, painter);
}

void Gauge::drawInnerCircle(QPainter& painter)
{
    hoverState->draw(*this, painter);
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

void HoverState::drawText(const Gauge& gauge,
                          QPainter& painter,
                          const QString& text)
{
    QFont font = painter.font();
    font.setPixelSize(static_cast<int>(0.3 * gauge.innerRect.width()));
    painter.setFont(font);
    painter.drawText(gauge.innerRect, Qt::AlignCenter, text);
}

void HoverStateHovered::draw(const Gauge& gauge, QPainter& painter)
{
    QString text = QString("%1").arg(gauge.actual);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);
    painter.drawEllipse(gauge.innerRect);
    HoverState::drawText(gauge, painter, text);
}

void HoverStateUnhovered::draw(const Gauge& gauge, QPainter& painter)
{
    QString text = QString("%1%").arg(
        gauge.goal != 0 ? gauge.actual * 100 / gauge.goal : 0);
    painter.setBrush(Gauge::backgroundFree);
    painter.drawEllipse(gauge.innerRect);
    painter.setPen(gauge.backgroundHovered);
    HoverState::drawText(gauge, painter, text);
}

void WorkProgressState::draw(const Gauge& gauge, QPainter& painter)
{
    setupBrushes();
    const int fullCircle = 360 * 16;
    const int offsetToTop = 90 * 16;
    int completedAngle = static_cast<int>(
        (gauge.actual % gauge.goal) * fullCircle / float(gauge.goal));
    painter.setBrush(empty);
    painter.drawEllipse(gauge.outerRect);
    painter.setBrush(filled);
    painter.drawPie(gauge.outerRect, offsetToTop, -completedAngle);
}

void WorkProgressState::setupBrushes() {}

void WorkProgressUnderwork::setupBrushes()
{
    empty = QBrush{Gauge::normalEmpty};
    filled = QBrush{Gauge::normalFilled};
}

void WorkProgressOverwork::setupBrushes()
{
    empty = QBrush{Gauge::overfilledEmpty};
    filled = QBrush{Gauge::overfilledFilled};
}

void WorkProgressDone::draw(const Gauge& gauge, QPainter& painter)
{
    painter.setBrush(Gauge::normalFilled);
    painter.drawEllipse(gauge.outerRect);
}

void WorkProgressNone::draw(const Gauge& gauge, QPainter& painter)
{
    painter.setBrush(Gauge::normalEmpty);
    painter.drawEllipse(gauge.outerRect);
}
