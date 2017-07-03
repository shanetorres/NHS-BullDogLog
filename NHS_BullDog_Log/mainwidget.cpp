#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QtCore>
#include <QtGui>

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);
}

mainWidget::~mainWidget()
{
    delete ui;
}



void mainWidget::on_adminButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void mainWidget::on_officerButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void mainWidget::on_quitButton_clicked()
{
    QApplication::quit();
}
