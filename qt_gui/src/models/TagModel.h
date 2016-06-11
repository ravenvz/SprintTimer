#ifndef TAGMODEL_H
#define TAGMODEL_H


#include "core/IPomodoroService.h"
#include "models/AsyncListModel.h"
#include <QStringListModel>


class TagModel : public AsyncListModel {

public:
    explicit TagModel(IPomodoroService& pomodoroService, QObject* parent = 0);

    bool setData(const QModelIndex& index,
                 const QVariant& value,
                 int role = Qt::EditRole) final;

    Qt::ItemFlags flags(const QModelIndex& index) const final;

    int rowCount(const QModelIndex& parent = QModelIndex()) const final;

    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const final;

protected:
    void requestDataUpdate() final;

private:
    std::vector<std::string> storage;
    IPomodoroService& pomodoroService;

    void onDataArrived(const std::vector<std::string>& tags);
};


#endif /* end of include guard: TAGMODEL_H */
