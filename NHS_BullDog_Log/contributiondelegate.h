#ifndef CONTRIBUTIONDELEGATE_H
#define CONTRIBUTIONDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QLineEdit>
#include "currentstudent.h"
#include <QVector>

class CurrentStudent;

class ContributionDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ContributionDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index);

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void eventEdited(QString, int, int) const;

public slots:
};

#endif // CONTRIBUTIONDELEGATE_H
