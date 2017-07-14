#include "officerdelegate.h"
#include <qDebug>
#include <iostream>


officerDelegate::officerDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *officerDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{

    QLineEdit *editor = new QLineEdit(parent);
    return editor;


}

void officerDelegate::setEditorData(QWidget *editor, const QModelIndex &index)
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);

}

void officerDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

       QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
       QString value = lineEdit->text();
       model->setData(index,value, Qt::EditRole);

       QVector<QString> studentData(totalData);

       //a vector of QStrings saves data at specific cells in the tableView
        for (int i = 0; i < totalData; ++i)
        {
            QModelIndex dataIndex = model->index(index.row(), i, QModelIndex());
            studentData[i] = dataIndex.model()->data(dataIndex, Qt::EditRole).toString();
        }

        //all student data is assigned to a specific value from the vector of strings
        QString studentFirstName = studentData[0];
        QString studentLastName = studentData[1];


        CurrentStudent student;
        student.setFirstName(studentFirstName);
        student.setLastName(studentLastName);

        qDebug() << "After" << student.getFirstName() << " " << student.getLastName();

       //a signal that sends the student object and row number to the mainwidget
       emit studentNameEdited(student, index.row());

}

void officerDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

