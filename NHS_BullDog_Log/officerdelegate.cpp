#include "officerdelegate.h"

officerDelegate::officerDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *officerDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

}

void officerDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
