#ifndef TAGMODEL_H
#define TAGMODEL_H


#include "core/IPomodoroService.h"
#include <QStringListModel>


class TagModel : public QStringListModel {

public:
    explicit TagModel(IPomodoroService& pomodoroService, QObject* parent = 0);

    bool setData(const QModelIndex& index,
                 const QVariant& value,
                 int role = Qt::EditRole) final;

    Qt::ItemFlags flags(const QModelIndex& index) const final;

    void requestData();

private:
    IPomodoroService& pomodoroService;

    void onDataArrived(const std::vector<std::string>& tags);
};


#endif /* end of include guard: TAGMODEL_H */
