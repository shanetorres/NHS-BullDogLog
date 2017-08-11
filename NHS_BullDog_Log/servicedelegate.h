#ifndef SERVICEDELEGATE_H
#define SERVICEDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QLineEdit>
#include "currentstudent.h"
#include <QVector>

class ServiceDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ServiceDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index);

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void serveEventEdited(QString, int, int) const;

public slots:
};

#endif // SERVICEDELEGATE_H
