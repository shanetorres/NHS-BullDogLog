#ifndef OFFICERSPINDELEGATE_H
#define OFFICERSPINDELEGATE_H

#include <QObject>
#include <QModelIndex>
#include <QItemDelegate>
#include <QSpinBox>
#include <QVector>
#include "currentstudent.h"

class OfficerSpinDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit OfficerSpinDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;


signals:
    void studentSpinBoxEdited(CurrentStudent, int) const;

public slots:

private:
    const int totalData = 3;
};

#endif // OFFICERSPINDELEGATE_H
