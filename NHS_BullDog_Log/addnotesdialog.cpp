#include "addnotesdialog.h"
#include "ui_addnotesdialog.h"
#include <QMessageBox>

addNotesDialog::addNotesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addNotesDialog)
{
    ui->setupUi(this);
}

addNotesDialog::~addNotesDialog()
{
    delete ui;
}

void addNotesDialog::reject()
{
    this->hide();
    emit cancelClicked();
}

void addNotesDialog::on_okButton_clicked()
{
    value = ui->notesEdit->text();
    emit notesEdited(value);
}

void addNotesDialog::on_cancelButton_clicked()
{
    emit cancelClicked();
}
