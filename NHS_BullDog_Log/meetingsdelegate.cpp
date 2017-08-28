#include "meetingsdelegate.h"

MeetingsDelegate::MeetingsDelegate(QObject *parent) : QItemDelegate(parent)
{
    Items.push_back(" ");
    Items.push_back("Absent");
    Items.push_back("Present");
}

QWidget *MeetingsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    for (int i  = 0; i < Items.size(); i++)
    {
        editor->addItem(Items[i]);
    }
    return editor;
}

void MeetingsDelegate::setEditorData(QWidget *editor, const QModelIndex &index)
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentText(value);
}

void MeetingsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    bool meetingStatus;
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    model->setData(index, comboBox->currentText(), Qt::EditRole);

    //sets the bool value of the student's attendance dependent on the selected item of the combo box
    if (comboBox->currentText() == "Present") { meetingStatus = true; }
    else if (comboBox->currentText() == "No") { meetingStatus = false; }
    else { meetingStatus = false; }
    emit meetingComboEdited(meetingStatus, index.row(), index.column());
}

void MeetingsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
