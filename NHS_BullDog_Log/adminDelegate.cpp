#include "admindelegate.h"
#include <qDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>


adminDelegate::adminDelegate(QObject *parent) : QItemDelegate(parent)
{
    //the text that is dispalyed in the combo box
    Items.push_back(" ");
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
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4 || index.column() == 5 || index.column() == 6)
    {
        QComboBox *editor = new QComboBox(parent);
        for (int i  = 0; i < Items.size(); i++)
        {
            editor->addItem(Items[i]);
        }
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
        editor->addItem(" ");
        editor->addItem("Applicant");
        editor->addItem("Member");
        return editor;
    }
    else if (index.column() == 9) {
        QLineEdit *editor = new QLineEdit(parent);
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
    else if(index.column() == 9)
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
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();
        model->setData(index,value, Qt::EditRole);
        emit prospectEdited(index.row());

    }
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4 || index.column() == 5 || index.column() == 6)     //data from combo boxes
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);
        emit prospectEdited(index.row());
        emit checkStudentPromo(index.row());
    }
    else if (index.column() == 7) {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);

        emit prospectEdited(index.row());
    }
    else if (index.column() == 8) {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);
        emit prospectEdited(index.row());
    }
    else if (index.column() == 9)         //data from the line edits is assigned to a student object and that object is then emitted
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();
        model->setData(index,value, Qt::EditRole);


        emit prospectEdited(index.row());

    }
}

void adminDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

