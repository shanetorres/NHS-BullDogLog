#include "addcontributiondialog.h"
#include "ui_addcontributiondialog.h"
#include <QMessageBox>

AddContributionDialog::AddContributionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContributionDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setStyleSheet("background-color: khaki; border-color: black;");
}

AddContributionDialog::~AddContributionDialog()
{
    delete ui;
}

void AddContributionDialog::reject()
{
    this->hide();
    emit cancelClicked();
}

void AddContributionDialog::on_okButton_clicked()
{
    value = ui->eventNameEdit->text();
    emit eventNameEdited(value);
}

void AddContributionDialog::on_cancelButton_clicked()
{
    emit cancelClicked();
}
