#include "officerspindelegate.h"
#include <QDebug>

OfficerSpinDelegate::OfficerSpinDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *OfficerSpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setMinimum(0);
    editor->setMaximum(100);
    return editor;
}

void OfficerSpinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

void OfficerSpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);

    QVector<int> studentData(totalData);

    for (int i = 0; i < totalData; i++)
    {
        QModelIndex dataIndex = model->index(index.row(), i + 2, QModelIndex());
        studentData[i] = dataIndex.model()->data(dataIndex, Qt::EditRole).toInt();
    }

    int studentContributions = studentData[0];
    int studentServProjects = studentData[1];
    int studentMeetings = studentData[2];

    CurrentStudent student;
    student.setContributions(studentContributions);
    student.setServProjects(studentServProjects);
    student.setAttendedMeetings(studentMeetings);
    qDebug() << "SPINBOX: " << student.getContributions() << " " << student.getServProjects() << " " << student.getAttendedMeetings();

    emit studentSpinBoxEdited(student, index.row());
}

void OfficerSpinDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
