/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
 * request for data update and data arrival. When data has arrived
 * and set, model emits 'updateFinished()' signal.
 *
 * Due to async nature, model has no means to know when it's
 * underlying data has been changed externally. To mitigate
 * this issue, 'synchronize()' slot is provided. When it's called,
 * model requests it's data, but does not emit 'updateFinished()'
 * signal to avoid chained update requests for it's subscribers
 * in case when they do operate on mutually dependent datasets. */
#ifdef _MSC_VER
class GLIB_EXPORT AsyncListModel : public QAbstractListModel {
#else
class AsyncListModel : public QAbstractListModel {
#endif // _MSV_VER
    Q_OBJECT

public:
    explicit AsyncListModel(QObject* parent);

public slots:
    /* Tells model to submit cached data to permanent storage.
     *
     * It has the same intent as QAbstractItemModel::submit(),
     * but a lot of Qt widgets call the latter as they see fit, and
     * debugging might be quite to find the caller to customize
     * behaviour. Hence this alternative.
     *
     * Default implementation does nothing.*/
    virtual void submitData();

    /* Tells model to discard cached information.
     *
     * It has the same intent as QAbstractItemModel::revert(),
     * but provided as an alternative to simplify behaviour
     * customization (as one can be sure that none of Qt objects
     * will trigger this slot).
     *
     * Default implementation does nothing.*/
    virtual void revertData();

    /* Request async data update. Upon receiving data  */
    virtual void requestDataUpdate() = 0;

    /* Request async data update but do not emit signal upon receiving data. */
    virtual void requestSilentDataUpdate() = 0;

signals:
    /* Emitted when model data is received.
     * It is not emitted when request for data update requested
     * via synchronize slot. */
    void updateFinished();
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: IASYNCMODEL_H_RSKAFU9C */
