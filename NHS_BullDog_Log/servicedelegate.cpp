#include "servicedelegate.h"

ServiceDelegate::ServiceDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *ServiceDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}

void ServiceDelegate::setEditorData(QWidget *editor, const QModelIndex &index)
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void ServiceDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();
    model->setData(index,value, Qt::EditRole);
    if (model->columnCount() > 2)
    {
            emit serveEventEdited(index.row(), index.column());
    }
}

void ServiceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
