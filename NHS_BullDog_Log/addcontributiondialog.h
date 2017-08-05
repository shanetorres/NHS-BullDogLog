#ifndef ADDCONTRIBUTIONDIALOG_H
#define ADDCONTRIBUTIONDIALOG_H

#include <QDialog>

namespace Ui {
class AddContributionDialog;
}

class AddContributionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddContributionDialog(QWidget *parent = 0);
    ~AddContributionDialog();
    void reject();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

signals:
    void eventNameEdited(QString);
    void cancelClicked();

private:
    Ui::AddContributionDialog *ui;
    QString value;
};

#endif // ADDCONTRIBUTIONDIALOG_H
