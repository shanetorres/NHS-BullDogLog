#ifndef MEETINGSDELEGATE_H
#define MEETINGSDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QComboBox>
#include "currentstudent.h"
#include <QVector>


class MeetingsDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MeetingsDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index);

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void meetingComboEdited(bool, int, int) const;

public slots:

private:
       std::vector<QString> Items;
};

#endif // MEETINGSDELEGATE_H
