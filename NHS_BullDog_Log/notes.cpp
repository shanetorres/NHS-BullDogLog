#include "notes.h"
#include "ui_notes.h"

notes::notes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::notes)
{
    ui->setupUi(this);
}

notes::~notes()
{
    delete ui;
}
