#ifndef ISTATISTICALCHARTLEGEND_H_BQJ9ZLKR
#define ISTATISTICALCHARTLEGEND_H_BQJ9ZLKR


#include <QWidget>
#include <QFont>
#include <QString>


class IStatisticalChartLegend : public QWidget {
    Q_OBJECT

public:
    IStatisticalChartLegend(QWidget* parent = 0)
        : QWidget(parent)
    {
    }
    virtual ~IStatisticalChartLegend();

    virtual void setData(const std::vector<std::string>& labels) = 0;

    virtual void
    setData(const std::vector<std::pair<std::string, double>>& labels)
        = 0;

    virtual void setTitle(const QString& title) = 0;

    virtual void setTitleFont(QFont font) = 0;

    virtual void toggleSelected(size_t index) = 0;

    virtual bool isSelected(size_t index) const = 0;


protected slots:
    virtual void onItemClicked(size_t index) = 0;

signals:
    void itemClicked(size_t index);
};

#endif /* end of include guard: ISTATISTICALCHARTLEGEND_H_BQJ9ZLKR */
