#ifndef NEWPOMOMODEL_H_UJXOKINE
#define NEWPOMOMODEL_H_UJXOKINE

#include <QStringListModel>

class NewPomoModel : public QStringListModel
{
public:
    NewPomoModel() = default;
    ~NewPomoModel() = default;

    NewPomoModel(NewPomoModel&&) = default;
    NewPomoModel& operator=(NewPomoModel&&) = default;

    NewPomoModel(const NewPomoModel&) = default;
    NewPomoModel& operator=(const NewPomoModel&) = default;

private:
    IPomodoroStorage storage;
    QStringList container;
};

#endif /* end of include guard: NEWPOMOMODEL_H_UJXOKINE */
