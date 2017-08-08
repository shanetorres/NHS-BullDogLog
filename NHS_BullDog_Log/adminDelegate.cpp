#include "admindelegate.h"
#include <qDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>


adminDelegate::adminDelegate(QObject *parent) : QItemDelegate(parent)
{
    //the text that is dispalyed in the combo box
    Items.push_back("No");
    Items.push_back("Yes");
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
        for (int i  = 0; i < Items.size(); i++)
        {
            editor->addItem(Items[i]);
        }
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
        emit studentNameEdited(student, index.row());

    }
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4 || index.column() == 5)     //data from spin boxes
    {
        ProspectStudent student;
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(value);

        QVector<QString> studentDataString(4);
        QVector<bool> studentDataBool(4);

        for (int i = 0; i < 4; i++)
        {
            QModelIndex dataIndex = model->index(index.row(), i + 2, QModelIndex());
            studentDataString[i] = dataIndex.model()->data(dataIndex, Qt::EditRole).toString();
            if (studentDataString[i] == "Yes")
            {
                studentDataBool[i] = true;
            }
            else
            {
                studentDataBool[i] = false;
            }
            emit studentComboEdited(student, index.row());
        }

        bool studentApplication = studentDataBool[0];
        bool studentEssay = studentDataBool[1];
        bool studentRecommendation = studentDataBool[2];
        bool studentApproval = studentDataBool[3];

        student.setApplicationBool(studentApplication);
        student.setEssayBool(studentEssay);
        student.setRecommendationBool(studentRecommendation);
        student.setApprovalBool(studentApproval);
    }
    else if (index.column() == 6)                       //data from combo box
    {
        ProspectStudent student;
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();
        model->setData(index,value, Qt::EditRole);
       //a vector of QStrings saves data at specific cells in the tableView

        QModelIndex dataIndex = model->index(index.row(), 6, QModelIndex());
        float studentGpa = dataIndex.model()->data(dataIndex, Qt::EditRole).toFloat();

        student.setStudentGpa(studentGpa);

        emit studentNameEdited(student, index.row());
    }


}

void adminDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
