#include "officercombodelegate.h"
#include <QDebug>

OfficerComboDelegate::OfficerComboDelegate(QObject *parent) : QItemDelegate(parent)
{
    Items.push_back(" ");
    Items.push_back("Yes");
    Items.push_back("No");
}

QWidget *OfficerComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QComboBox *editor = new QComboBox(parent);
    for (int i  = 0; i < Items.size(); i++)
    {
        editor->addItem(Items[i]);
    }
    return editor;
}

void OfficerComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = index.model()->data(index, Qt::EditRole).toString();
    comboBox->setCurrentText(value);
    CurrentStudent student;

    //sets the bool value of the student's attendance dependent on the selected item of the combo box
    if (value == "Yes") { student.setInductionAttendance(true); }
    else if (value == "No") { student.setInductionAttendance(false); }
    else { student.setInductionAttendance(false); }

    emit studentComboBoxEdited(student, index.row());
}

void OfficerComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void OfficerComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
