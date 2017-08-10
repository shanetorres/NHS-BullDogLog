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
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4 || index.column() == 5 || index.column() == 6)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem("Yes");
        editor->addItem("No");
        return editor;
    }
    else if (index.column() == 7) {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setMinimum(11);
        editor->setMaximum(12);
        return editor;
    }
    else if (index.column() == 8) {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem("Applicant");
        editor->addItem("Member");
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
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4 || index.column() == 5 || index.column() == 6)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(value);
    }
    else if (index.column() == 7) {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    }
    else if (index.column() == 8) {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(value);
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
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4 || index.column() == 5 || index.column() == 6)     //data from combo boxes
    {
        ProspectStudent student;
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);

        QVector<bool> studentRequirements(5);
        QVector<QString> studentDataString(5);

        for (int i = 0; i < 5; i++)
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
        student.setStudentGpa(studentRequirements[4]);

        qDebug() << "COMBOBOX: " << student.getApplicationBool() << " " << student.getEssayBool() << " " << student.getRecommendationBool() << " "
                 << student.getApprovalBool() << student.getStudentGpa();

        emit studentComboEdited_2(student, index.row());
    }
    else if (index.column() == 7) {
        ProspectStudent student;
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);

        QModelIndex dataIndex = model->index(index.row(), 7, QModelIndex());
        int studentDataInt = dataIndex.model()->data(dataIndex, Qt::EditRole).toInt();

        student.setStudentClass(studentDataInt);

        qDebug() << "SPINBOX: " << student.getStudentClass();
        emit studentClassEdited(student, index.row());
    }
    else if (index.column() == 8) {
        ProspectStudent student;
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);

        //sets the bool value of the student's attendance dependent on the selected item of the combo box
        if (comboBox->currentText() == "Member") { student.setStudentStatus(true); }
        else if (comboBox->currentText() == "Applicant") { student.setStudentStatus(false); }
        else { student.setStudentStatus(false); }
        emit studentStatusEdited(student, index.row());
    }


}

void adminDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

