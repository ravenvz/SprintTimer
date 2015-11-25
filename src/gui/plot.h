#ifndef PLOT_H
#define PLOT_H


#include <QString>
#include <QPainter>
#include <QWidget>
#include <QEvent>

#include <QDebug>


struct GraphPoint
{
    double x;
    double y;
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
    using PointBoxData = QVector<QRectF>;
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
    void showEvent(QShowEvent*) override;

signals:
    void sizeComputed();

private slots:
    void onSizeComputed();

private:
    QVector<Graph> graphs;
    QVector<PointBoxData> graphPointBoxes;
    AxisRange rangeX;
    AxisRange rangeY;
    QRectF availableRect;
    bool adaptiveSizeComputed = false;

    void computeAdaptiveSizes();
    void constructPointBoxes();
};


#endif // PLOT_H
