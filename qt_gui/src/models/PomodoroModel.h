#ifndef POMODOROMODELNEW_H_MQZ2XAPI
#define POMODOROMODELNEW_H_MQZ2XAPI

#include "core/IPomodoroService.h"
#include "models/AsyncListModel.h"
#include <memory>
#include <vector>

class PomodoroModel : public AsyncListModel {
    Q_OBJECT
public:
    explicit PomodoroModel(IPomodoroService& pomodoroService,
                           QObject* parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const final;

    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const final;

    void setDateFilter(const TimeSpan& timeSpan);

    void insert(const TimeSpan& timeSpan, const std::string& taskUuid);

    void remove(int row);

protected:
    void requestDataUpdate() final;

private:
    std::vector<Pomodoro> storage;
    TimeSpan interval;
    IPomodoroService& pomodoroService;

    void onDataChanged(const std::vector<Pomodoro>& items);
};

#endif /* end of include guard: POMODOROMODELNEW_H_MQZ2XAPI */
