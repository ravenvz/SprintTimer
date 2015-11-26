#ifndef PLOT_H
#define PLOT_H


#include <QString>
#include <QPainter>
#include <QWidget>
#include <QEvent>
#include <QGraphicsScene>

#include <QDebug>


struct GraphPoint
{
    double x;
    double y;
    QString label;
};

class PointBox
{
public:
    void setPath(const QPainterPath &path);
    void setToolTip(const QString &toolTip);
    void setPosition(const QPoint &position);
    void setColor(const QColor &color);

    QPainterPath path() const;
    QPoint position() const;
    QColor color() const;
    QString toolTip() const;

private:
    QPainterPath mPath;
    QPoint mPosition;
    QColor mColor;
    QString mToolTip;
};


using GraphData = QVector<GraphPoint>;


class Graph
{
public:
    using const_iterator = GraphData::const_iterator;
    Graph();
    virtual ~Graph() = default;
    void setData(GraphData& data);
    void setPen(QPen& pen);
    const QPen getPen() const;
    const GraphPoint& operator[](std::size_t idx) const;
    void clearData();
    void setShowPoints(bool showPoints);
    bool showPoints() const;
    const_iterator cbegin() const { 
        return points.cbegin();
    }
    const_iterator cend() const {
        return points.cend();
    }
    size_t size() const;

private:
    QPen pen;
    GraphData points;
    bool mShowPoints = false;

};


struct AxisRange {
    double start;
    double end;

    void setRange(double start, double end) {
        this->start = std::min(start, end);
        this->end = std::max(start, end);
    }

    double getSpan() const {
        return end - start;
    }
};


class Plot : public QWidget
{
    Q_OBJECT

public:
    // using PointBoxData = QVector<QRectF>;
    using PointPixelCoordinates = QVector<QPointF>;
    explicit Plot(QWidget* parent = 0);
    virtual ~Plot () = default;
    void reset();
    void addGraph(Graph& graph);
    void setGraphData(int graphNum, GraphData& data);
    void replot();
    void setRangeX(double start, double end);
    void setRangeY(double start, double end);

protected:
    void paintEvent(QPaintEvent*) override;
    void mouseMoveEvent(QMouseEvent* event) override;

signals:
    void sizeComputed();

private slots:
    void onSizeComputed();

private:
    QVector<Graph> graphs;
    QVector<PointPixelCoordinates> translatedPoints;
    // QVector<PointBoxData> graphPointBoxes;
    AxisRange rangeX;
    AxisRange rangeY;
    QRectF availableRect;
    bool adaptiveSizeComputed = false;
    double pointBoxSize;

    void computeAdaptiveSizes();
    void constructPointBoxes();
    std::pair<int, int> pointBoxContain(const QPoint& pos) const;
};


#endif // PLOT_H
