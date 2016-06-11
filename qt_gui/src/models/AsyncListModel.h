#ifndef IASYNCMODEL_H_RSKAFU9C
#define IASYNCMODEL_H_RSKAFU9C

#include <QAbstractListModel>
#include <QObject>

/* Model that updates it' s data asyncroniously.
 *
 * Async data update means that there is a unknown delay between
 * request for data update and data arrival. When data has arrived
 * and set, model emits 'updateFinished()' signal.
 *
 * Due to async nature, model has no means to know when it's
 * underlying data has been changed externally. To mitigate
 * this issue, 'synchronize()' slot is provided. When it's called,
 * model requests it's data, but does not emit 'updateFinished()'
 * signal to avoid chained update requests for it's subscribers
 * in case when they do operate on mutually dependent datasets. */
class AsyncListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit AsyncListModel(QObject* parent);

public slots:
    /* Request to update model data, but do not broadcast updateFinished
     * signal when data is received. */
    virtual void synchronize();

protected:
    bool silent{false};

    /* Request async data update. */
    virtual void requestDataUpdate() = 0;

    /* Emit updateFinished signal when silent flag is false. */
    virtual void broadcastUpdateFinished();

signals:
    /* Emitted when model data is received.
     * It is not emitted when request for data update requested
     * via synchronize slot. */
    void updateFinished();
};

#endif /* end of include guard: IASYNCMODEL_H_RSKAFU9C */
