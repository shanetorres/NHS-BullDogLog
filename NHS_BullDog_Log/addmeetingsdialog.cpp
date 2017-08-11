#include "addmeetingsdialog.h"
#include "ui_addmeetingsdialog.h"
#include <QDebug>

AddMeetingsDialog::AddMeetingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMeetingsDialog)
{
    ui->setupUi(this);
}

AddMeetingsDialog::~AddMeetingsDialog()
{
    delete ui;
}

void AddMeetingsDialog::on_okMeetingButton_clicked()
{
    value = ui->dateEdit->date().toString();
    dateAdded(value);
}