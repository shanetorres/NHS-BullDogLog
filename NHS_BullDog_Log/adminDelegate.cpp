#include "admindelegate.h"
#include <qDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>


adminDelegate::adminDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *adminDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{

    //control statement determines which editor to create and return depenedent on the column number
    if(index.column() == 0 || index.column() == 1)
    {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4 || index.column() == 5)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem("No");
        editor->addItem("Yes");
        return editor;
    }
    else if (index.column() == 6)
    {
        QLineEdit *editor = new QLineEdit(parent);
        editor->setMaxLength(4);
        return editor;
    }
}

void adminDelegate::setEditorData(QWidget *editor, const QModelIndex &index)
{

    if(index.column() == 0 || index.column() == 1)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value);
    }
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4 || index.column() == 5)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(value);
    }
    else if (index.column() == 6)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value);
    }

}

void adminDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == 0 || index.column() == 1)         //data from the line edits is assigned to a student object and that object is then emitted
    {
        ProspectStudent student;
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();
        model->setData(index,value, Qt::EditRole);
        QVector<QString> studentData(2);
       //a vector of QStrings saves data at specific cells in the tableView
        for (int i = 0; i < 2; ++i)
        {
            QModelIndex dataIndex = model->index(index.row(), i, QModelIndex());
            studentData[i] = dataIndex.model()->data(dataIndex, Qt::EditRole).toString();
        }

        //all student data is assigned to a specific value from the vector of strings
        QString studentFirstName = studentData[0];
        QString studentLastName = studentData[1];

        student.setFirstName(studentFirstName);
        student.setLastName(studentLastName);

        qDebug() << "After" << student.getFirstName() << " " << student.getLastName();
        emit studentNameEdited_2(student, index.row());

    }
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4 || index.column() == 5)     //data from combo boxes
    {
        ProspectStudent student;
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);

        QVector<bool> studentRequirements(4);
        QVector<QString> studentDataString(4);

        for (int i = 0; i < 4; i++)
        {
            QModelIndex dataIndex = model->index(index.row(), i + 2, QModelIndex());
            studentDataString[i] = dataIndex.model()->data(dataIndex, Qt::EditRole).toString();
            if (studentDataString[i] == "Yes") {
                studentRequirements[i] = true;
            }
            else if (studentDataString[i] == "No") {
                studentRequirements[i] = false;
            }
        }

        student.setApplicationBool(studentRequirements[0]);
        student.setEssayBool(studentRequirements[1]);
        student.setRecommendationBool(studentRequirements[2]);
        student.setApprovalBool(studentRequirements[3]);

        qDebug() << "COMBOBOX: " << student.getApplicationBool() << " " << student.getEssayBool() << " " << student.getRecommendationBool() << " " << student.getApprovalBool();

        emit studentComboEdited_2(student, index.row());
    }
    else if (index.column() == 6)                       //data from combo box
    {
        ProspectStudent student;
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();
        model->setData(index,value, Qt::EditRole);
       //a vector of QStrings saves data at specific cells in the tableView

        QModelIndex dataIndex = model->index(index.row(), 6, QModelIndex());
        QString studentGpa = dataIndex.model()->data(dataIndex, Qt::EditRole).toString();

        student.setStudentGpa(studentGpa);

        qDebug() << "After" << student.getFirstName() << " " << student.getLastName() << " " << student.getStudentGpa();

        emit studentGpaEdited(student, index.row());
    }


}

void adminDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
