#ifndef ADDNOTESDIALOG_H
#define ADDNOTESDIALOG_H

#include <QDialog>

namespace Ui {
class addNotesDialog;
}

class addNotesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addNotesDialog(QWidget *parent = 0);
    ~addNotesDialog();
    void reject();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

signals:
    void notesEdited(QString);
    void cancelClicked();

private:
    Ui::addNotesDialog *ui;
    QString value;
};

#endif // ADDNOTESDIALOG_H
