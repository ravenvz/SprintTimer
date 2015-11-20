#ifndef PLOT_H
#define PLOT_H


using GraphData = QVector<GraphPoint>;

struct GraphPoint
{
    double xValue;
    double yValue;
};


class Graph
{
public:
    Graph();
    virtual ~Graph();
    void setData(GraphData& data);
    void setPen(QPen& pen);
    void setLabels(QVector<QString>& labels);
    const QPen pen() const;
    const GraphPoint& operator[](std::size_t idx) const;

private:
    QPen pen;
    GraphData points;
    QVector<QString> labels;

};


class Plot : public QWidget
{
    Q_OBJECT

public:
    explicit Plot(QWidget* parent = 0);
    virtual ~Plot () = default;
    void reset();
    void addGraph(Graph& graph);

private:
    QVector<Graph> graphs;

};


#endif // PLOT_H
