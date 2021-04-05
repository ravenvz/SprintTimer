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
#ifndef IASYNCMODEL_H_RSKAFU9C
#define IASYNCMODEL_H_RSKAFU9C

#include <QAbstractListModel>
#include <QObject>

#ifdef _MSC_VER
#include "qt_gui/WinExport.h"
#endif // _MSC_VER

namespace sprint_timer::ui::qt_gui {

/* Model that updates it's data asyncroniously.
 *
 * Async data update implies a delay between
 * request for data update and data arrival.
 *
 * Due to async nature, model has no means to know when it's
 * underlying data has been changed externally. To mitigate
 * this issue, two slots are provided to sync data manually:
 *
 * requestDataUpdate() - to request data and emit updateFinished signal upon
 *                       data arrival
 *
 * requestSilentDataUpdate() - to request data but do not emit signal upon
 *                             data arrival - handy when you do not want to
 *                             notify updateFinished signal listener, i.e. to
 *                             prevent chained data updates
 *
 */
#ifdef _MSC_VER
class GLIB_EXPORT AsyncListModel : public QAbstractListModel {
#else
class AsyncListModel : public QAbstractListModel {
#endif // _MSV_VER
    Q_OBJECT

public:
    explicit AsyncListModel(QObject* parent);

    /* Request async data update. Upon receiving data updateFinished() signal
     * is emitted. */
    void requestDataUpdate();

    /* Request async data update but do not emit signal upon receiving data. */
    void requestSilentDataUpdate();

protected:
    /* Subclasses supposed to implement this method to query storage for data.
     */
    virtual void requestUpdate() = 0;

signals:
    /* Signals that previously requested data is arrived. */
    void updateFinished();
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: IASYNCMODEL_H_RSKAFU9C */
