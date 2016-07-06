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
#ifndef IDATAWIDGET_H_DMUO9TSG
#define IDATAWIDGET_H_DMUO9TSG

#include <QWidget>

/* Widget that displays some data (like statistics, etc) and has
 * means to access this data, but does not now when underlying data
 * has been changed.
 *
 * Extends QWidget to provide 'synchronize()' slot.
 * It's ment to be called when widget's underlying data
 * has been changed and should be requeried.*/
class DataWidget : public QWidget {
    Q_OBJECT

public:
    DataWidget(QWidget* parent);

    virtual ~DataWidget() = default;

public slots:
    virtual void synchronize() = 0;
};

#endif /* end of include guard: IDATAWIDGET_H_DMUO9TSG */
