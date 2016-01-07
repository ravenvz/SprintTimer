#ifndef POMODOROQUERYMODEL_H
#define POMODOROQUERYMODEL_H

#include <QSqlTableModel>
#include <QSqlQuery>
#include <functional>
#include "core/entities/Pomodoro.h"

class PomodoroModel : public QSqlTableModel
{
public:
    explicit PomodoroModel(QObject* parent = 0);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    // bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    void removePomodoro(const QModelIndex& index);
    void insertPomodoro(const Pomodoro& pomodoro, long long associatedTodoItemId);

    void setRemovePomodoroFunctor(std::function<void (QVariant pomoId)> func);
    void setInsertPomodoroFunctor(std::function<bool (const Pomodoro& pomodoro, long long associatedTodoItemId)> func);

    enum class Columns {
        Id = 0,
        TodoId,
        Name,
        Tags,
        StartTime,
        FinishTime,
    };


private:
    std::function<void (QVariant pomoId)> removePomodoroFunctor;
    std::function<bool (const Pomodoro& pomodoro, long long associatedTodoItemId)> insertPomodoroFunctor;

    Pomodoro rowToPomodoro(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant columnData(const QModelIndex& index, const Columns& column, int role = Qt::DisplayRole) const;

};



#endif /* end of include guard: POMODOROQUERYMODEL_H */
