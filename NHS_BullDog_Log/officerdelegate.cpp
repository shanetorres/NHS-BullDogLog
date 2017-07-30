#include "officerdelegate.h"
#include <qDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>


officerDelegate::officerDelegate(QObject *parent) : QItemDelegate(parent)
{
    //the text that is dispalyed in the combo box
    Items.push_back(" ");
    Items.push_back("Yes");
    Items.push_back("No");
}

QWidget *officerDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
    //control statement determines which editor to creat and return depenedent on the column number
    if(index.column() == 0 || index.column() == 1)
    {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4)
    {
        QSpinBox *editor = new QSpinBox(parent);

        return editor;
    }
    else if (index.column() == 5)
    {
        QComboBox *editor = new QComboBox(parent);
        for (int i  = 0; i < Items.size(); i++)
        {
            editor->addItem(Items[i]);
        }
        return editor;
    }
}

void officerDelegate::setEditorData(QWidget *editor, const QModelIndex &index)
{

    if(index.column() == 0 || index.column() == 1)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value);
    }
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4)
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    }
    else if (index.column() == 5)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(value);
    }

}

void officerDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == 0 || index.column() == 1)         //data from the line edits is assigned to a student object and that object is then emitted
    {
        CurrentStudent student;
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
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4)     //data from spin boxes
    {
        CurrentStudent student;
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);

        QVector<int> studentDataInt(3);

        for (int i = 0; i < 3; i++)
        {
            QModelIndex dataIndex = model->index(index.row(), i + 2, QModelIndex());
            studentDataInt[i] = dataIndex.model()->data(dataIndex, Qt::EditRole).toInt();
        }

        int studentContributions = studentDataInt[0];
        int studentServProjects = studentDataInt[1];
        int studentMeetings = studentDataInt[2];

        student.setContributions(studentContributions);
        student.setServProjects(studentServProjects);
        student.setAttendedMeetings(studentMeetings);

        qDebug() << "SPINBOX: " << student.getContributions() << " " << student.getServProjects() << " " << student.getAttendedMeetings();
        emit studentSpinEdited(student, index.row());
    }
    else if (index.column() == 5)                       //data from combo box
    {
        CurrentStudent student;
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);

        //sets the bool value of the student's attendance dependent on the selected item of the combo box
        if (comboBox->currentText() == "Yes") { student.setInductionAttendance(true); }
        else if (comboBox->currentText() == "No") { student.setInductionAttendance(false); }
        else { student.setInductionAttendance(false); }
        emit studentComboEdited(student, index.row());
    }


}

void officerDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

