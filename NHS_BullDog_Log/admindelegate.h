#ifndef ADMINDELEGATE_H
#define ADMINDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QLineEdit>
#include <QString>
#include "prospectstudent.h"
#include <vector>
#include <QVector>
#include <QComboBox>

class ProspectStudent;

class adminDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit adminDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index);

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void studentNameEdited(ProspectStudent, int) const;

    void studentSpinEdited(ProspectStudent, int) const;

    void studentComboEdited(ProspectStudent, int) const;


public slots:

private:
   std::vector<QString> Items;

};

#endif // ADMINDELEGATE_H
