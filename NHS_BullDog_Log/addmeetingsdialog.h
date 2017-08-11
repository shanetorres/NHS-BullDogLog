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

private slots:
    void on_okMeetingButton_clicked();

signals:
    void dateAdded(QString);

private:
    Ui::AddMeetingsDialog *ui;
    QString value;
};

#endif // ADDMEETINGSDIALOG_H
