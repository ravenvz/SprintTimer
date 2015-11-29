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

private:
    struct PointBox {
        QPainterPath path;
        QPointF position;
        QString toolTip;
        QString label;
    };

public:
    using PointBoxContainer = QVector<PointBox>;
    explicit Plot(QWidget* parent = 0);
    virtual ~Plot() = default;
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
    QVector<PointBoxContainer> pointBoxes;
    AxisRange rangeX;
    AxisRange rangeY;
    QRectF availableRect;
    bool adaptiveSizeComputed {false};
    double pointBoxSize;
    const QBrush pointBoxBrush {Qt::white};
    constexpr static int labelSkipInd {28};
    constexpr static int toolTipOffset {10};
    // RelSizes' are relative to Widget's rect() height
    constexpr static double labelOffsetRelSize {0.15};
    constexpr static double marginRelSize {0.07};
    constexpr static double pointBoxRelSize {0.025};

    void computeAdaptiveSizes();
    void constructPointBoxes();
    const QString getPosTooltip(const QPoint& pos) const;
};


#endif // PLOT_H
