#include "contributiondelegate.h"
#include <qDebug>
#include <QModelIndex>

ContributionDelegate::ContributionDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *ContributionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}

void ContributionDelegate::setEditorData(QWidget *editor, const QModelIndex &index)
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void ContributionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();
    model->setData(index,value, Qt::EditRole);
    if (model->columnCount() > 2)
    {
        //if the column being edited is not the first or last name, then the data within the line edit is emitted
        if (index.column() != 0 && index.column() != 1)
        {
            QModelIndex dataIndex = model->index(index.row(), index.column(), QModelIndex());
            QString event = dataIndex.model()->data(dataIndex, Qt::EditRole).toString();
            emit eventEdited(event, index.row(), index.column());
        }
    }
}

void ContributionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
