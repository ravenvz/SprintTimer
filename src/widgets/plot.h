#ifndef PLOT_H
#define PLOT_H

#include <QString>
#include <QPainter>
#include <QWidget>
#include <QEvent>
#include <QGraphicsScene>

struct GraphPoint;

using GraphData = QVector<GraphPoint>;

class Graph;

struct AxisRange {
    // Set minimum and maximum values on axis.
    // If start > end, then start is asssigned to end, and end to start.
    void setRange(double start, double end);

    // Return axis span, that is distance between minimum and maximum value.
    double span() const;

private:
    double start;
    double end;
};

class Plot : public QWidget {
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

    // Add Graph to plot. Multiple graphs can be added.
    void addGraph(Graph& graph);

    // Set data points to graph with given number.
    void setGraphData(int graphNum, GraphData& data);

    // Clear data points from all graphs in plot.
    // Empty graphs remain attached to Plot.
    void reset();

    // Repaint plot. This method is ment to be called when graph data is
    // changed.
    void replot();

    // Set visible axis range.
    void setRangeX(double start, double end);

    // Set visible axis range.
    void setRangeY(double start, double end);

protected:
    // Override to paint graphs.
    void paintEvent(QPaintEvent*) override;

    // Override to show tooltip with point data when hovering over the point.
    void mouseMoveEvent(QMouseEvent* event) override;

signals:

    // Signal that is emitted when size of plot graphic primitives is computed.
    // Size of plot depends on parent widget.
    void sizeComputed();

private slots:

    // Compute data required to draw points.
    // Fired when plot graphic primitives' sizes are computed.
    void onSizeComputed();

private:
    QVector<Graph> graphs;
    QVector<PointBoxContainer> pointBoxes;
    AxisRange rangeX;
    AxisRange rangeY;
    QRectF availableRect;
    bool adaptiveSizeComputed{false};
    double pointBoxSize;
    const QBrush pointBoxBrush{Qt::white};
    constexpr static int labelSkipInd{28};
    constexpr static int toolTipOffset{10};
    // RelSizes' are relative to Widget's rect() height
    constexpr static double labelOffsetRelSize{0.15};
    constexpr static double marginRelSize{0.07};
    constexpr static double pointBoxRelSize{0.025};

    // Compute sizes of plot graphic primitives that depend on size of Widget.
    void computeAdaptiveSizes();

    // Compute data required to draw points relative to plot size.
    void constructPointBoxes();

    // Paint graph with given number.
    void paintGraph(const int graphNum, QPainter& painter) const;

    // Paint graph points and labels.
    void paintPoints(const PointBoxContainer& boxes, QPainter& painter) const;

    // Show tooltip with point data when hovering over a point on a plot.
    const QString getPosTooltip(const QPoint& pos) const;
};

struct GraphPoint {
    double x;
    double y;
    QString label;
};

class Graph {
public:
    using const_iterator = GraphData::const_iterator;

    // Set data for this Graph.
    void setData(GraphData& data);

    // Set pen for drawing graph lines.
    void setPen(QPen& pen);

    // Return pen that is currently used for drawing graph lines.
    const QPen pen() const;

    // Overload for subscript operator to get access to graph points data.
    // TODO change int to std::size_t when switched to std::vector
    const GraphPoint& operator[](int idx) const;

    // Clear graph points.
    void clearData();

    // Draw data points and labels on graph if showPoints is true.
    // Draw graph only if showPoints is false.
    void setShowPoints(bool showPoints);

    // Return true if data points are supposed to be drawn and false otherwise.
    bool showPoints() const;

    // Return iterator to the beginning (first data point)
    const_iterator cbegin() const;

    // Return iterator to the end.
    const_iterator cend() const;

    // Return number of points in graph.
    // TODO change int to std::size_t when switched to std::vector
    int size() const;

private:
    QPen mPen;
    GraphData points;
    bool mShowPoints = false;
};

#endif // PLOT_H
