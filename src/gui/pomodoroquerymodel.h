#ifndef POMODOROQUERYMODEL_H
#define POMODOROQUERYMODEL_H

#include <QSqlQueryModel>
#include "core/entities/Pomodoro.h"

class PomodoroQueryModel : public QSqlQueryModel
{
public:
    explicit PomodoroQueryModel(QObject* parent = 0);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    void removePomodoro(const QModelIndex& index);
    void refresh();
    enum class Columns {
        Id = 0,
        TodoId,
        Name,
        StartTime,
        FinishTime,
        Tags
    };


private:
    Pomodoro rowToPomodoro(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant columnData(const QModelIndex& index, const Columns& column, int role = Qt::DisplayRole) const;

};



#endif /* end of include guard: POMODOROQUERYMODEL_H */
