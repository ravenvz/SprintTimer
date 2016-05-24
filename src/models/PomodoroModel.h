#ifndef POMODOROMODELNEW_H_MQZ2XAPI
#define POMODOROMODELNEW_H_MQZ2XAPI

#include "core/use_cases/use_cases.h"
#include <QAbstractListModel>
#include <memory>
#include <vector>

class PomodoroModel : public QAbstractListModel {
public:
    explicit PomodoroModel(CoreApi::PomodoroCoreFacade& pomodoroService,
                           QObject* parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const final;

    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const final;

    void setDateFilter(const TimeSpan& timeSpan);

    void insert(const Pomodoro& pomodoro);

    // void remove(long long pomodoroId);
    void remove(int row);

private:
    std::vector<Pomodoro> storage;
    TimeSpan interval;
    CoreApi::PomodoroCoreFacade& pomodoroService;

    void retrieveData();

    void onDataChanged(const std::vector<Pomodoro>& items);
};

#endif /* end of include guard: POMODOROMODELNEW_H_MQZ2XAPI */
