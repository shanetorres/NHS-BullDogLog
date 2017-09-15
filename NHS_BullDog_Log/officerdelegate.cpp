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
        editor->setMinimum(0);
        editor->setMaximum(10);
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
    else if (index.column() == 6)
    {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setMinimum(11);
        editor->setMaximum(12);
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
    else if (index.column() == 6)
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    }
}

void officerDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == 0 || index.column() == 1)         //data from the line edits is assigned to a student object and that object is then emitted
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();
        model->setData(index,value, Qt::EditRole);
        emit studentEdited(index.row());

    }
    else if (index.column() == 2 || index.column() == 3 || index.column() == 4)     //data from spin boxes
    {

        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
        emit studentEdited(index.row());
    }
    else if (index.column() == 5)                       //data from combo box
    {

        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);
        emit studentEdited(index.row());
    }
    else if (index.column() == 6)
    {

        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
        emit studentEdited(index.row());
    }

}

void officerDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

