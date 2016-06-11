#ifndef IASYNCMODEL_H_RSKAFU9C
#define IASYNCMODEL_H_RSKAFU9C

#include <QAbstractListModel>
#include <QObject>

/* Model that updates it' s data asyncroniously.
 *
 * Due to async nature, model has no means to know when it's
 * underlying data has been changed. */
class AsyncListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit AsyncListModel(QObject* parent);

    // virtual ~AsyncListModel() = default;

public slots:
    /* Update model data, but do not broadcast updateFinished
     * signal when data is received. */
    virtual void synchronize();

protected:
    bool silent{false};

    virtual void requestDataUpdate() = 0;

    virtual void broadcastUpdateFinished();

signals:

    void updateFinished();
};

#endif /* end of include guard: IASYNCMODEL_H_RSKAFU9C */
