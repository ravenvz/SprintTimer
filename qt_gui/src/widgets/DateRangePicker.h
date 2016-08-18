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
#ifndef PICKPERIODWIDGET_H
#define PICKPERIODWIDGET_H

#include "dialogs/datepickdialog.h"
#include <QPointer>
#include <QStringListModel>

namespace Ui {
class DateRangePicker;
}


class DateRangePicker : public QWidget {

    Q_OBJECT

public:
    explicit DateRangePicker(QWidget* parent);
    ~DateRangePicker();
    void setInterval(DateInterval&& timeSpan);
    DateInterval getInterval() const;
    void setYears(const std::vector<std::string>& years);

signals:
    void timeSpanChanged(DateInterval newInterval);

private slots:
    void openDatePickDialog();
    void updateInterval();
    void updateInterval(DateInterval timeSpan);
    void updateSelectionHintLabel();

private:
    Ui::DateRangePicker* ui;
    DateInterval selectedInterval;
    QPointer<QStringListModel> yearsModel;
    QPointer<QStringListModel> monthsModel;

    void connectSlots();
};


#endif // PICKPERIODWIDGET_H
