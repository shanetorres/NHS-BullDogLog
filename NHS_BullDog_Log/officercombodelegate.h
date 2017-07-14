#ifndef OFFICERCOMBODELEGATE_H
#define OFFICERCOMBODELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QComboBox>
#include <vector>
#include "currentstudent.h"

class OfficerComboDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit OfficerComboDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void studentComboBoxEdited(CurrentStudent, int) const;

public slots:

private:
    std::vector<QString> Items;
};

#endif // OFFICERCOMBODELEGATE_H
