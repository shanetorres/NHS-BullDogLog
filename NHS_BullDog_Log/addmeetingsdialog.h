#ifndef ADDMEETINGSDIALOG_H
#define ADDMEETINGSDIALOG_H

#include <QDialog>

namespace Ui {
class AddMeetingsDialog;
}

class AddMeetingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMeetingsDialog(QWidget *parent = 0);
    ~AddMeetingsDialog();
    void reject();

private slots:
    void on_okMeetingButton_clicked();

    void on_cancelMeetingButton_clicked();

signals:
    void dateAdded(QString);
    void cancelClicked();

private:
    Ui::AddMeetingsDialog *ui;
    QString value;
};

#endif // ADDMEETINGSDIALOG_H
