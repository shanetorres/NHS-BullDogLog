#ifndef OFFICERDELEGATE_H
#define OFFICERDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QLineEdit>
#include <QString>
#include "currentstudent.h"
#include <vector>
#include <QVector>
#include <QSpinBox>
#include <QComboBox>




class CurrentStudent;

class officerDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit officerDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index);

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void studentNameEdited(CurrentStudent, int) const;

    void studentSpinEdited(CurrentStudent, int) const;

    void studentComboEdited(CurrentStudent, int) const;

    void studentGradeEdited(CurrentStudent, int) const;


public slots:

private:
   std::vector<QString> Items;

};

#endif // OFFICERDELEGATE_H
