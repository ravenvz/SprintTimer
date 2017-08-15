/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef ISTATISTICALCHARTLEGEND_H_BQJ9ZLKR
#define ISTATISTICALCHARTLEGEND_H_BQJ9ZLKR


#include <QWidget>
#include <QFont>
#include <QString>


class IStatisticalChartLegend : public QWidget {
    Q_OBJECT

public:
    IStatisticalChartLegend(QWidget* parent)
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
