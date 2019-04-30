/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#ifndef PICKPERIODWIDGET_H
#define PICKPERIODWIDGET_H

#include "qt_gui/dialogs/DateRangePickDialog.h"
#include <QStringListModel>
#include <core/IConfig.h>
#include <memory>

namespace Ui {
class DateRangePicker;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class DateRangePicker : public QWidget {

    Q_OBJECT

public:
    explicit DateRangePicker(QWidget* parent);
    ~DateRangePicker() override;
    void setInterval(DateInterval&& timeSpan);
    DateInterval getInterval() const;
    void setYears(const std::vector<std::string>& years);
    void setFirstDayOfWeek(dw::Weekday firstDayOfWeek_);

signals:
    void timeSpanChanged(DateInterval newInterval);

private slots:
    void openDatePickDialog();
    void updateInterval();
    void updateInterval(DateInterval timeSpan);
    void updateSelectionHintLabel();

private:
    std::unique_ptr<Ui::DateRangePicker> ui;
    dw::Weekday firstDayOfWeek;
    DateInterval selectedInterval;
    std::unique_ptr<QStringListModel> yearsModel;
    std::unique_ptr<QStringListModel> monthsModel;
};

} // namespace sprint_timer::ui::qt_gui


#endif // PICKPERIODWIDGET_H
