#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QtCore>
#include <QtGui>

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);

    /*ADMIN RECORDS*/

    /*OFFICER RECORDS*/

    //creating the model for all current students and setting resizing parameters for the view
    currentStudentsModel = new QStandardItemModel(this);
    ui->currentTableView->setModel(currentStudentsModel);
    ui->currentTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //setting the header text of the model
    currentStudentsModel->setHorizontalHeaderItem(0, new QStandardItem(QString("First Name")));
    currentStudentsModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Last Name")));
    currentStudentsModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Contributions")));
    currentStudentsModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Service Projects")));
    currentStudentsModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Meetings Attended")));
    currentStudentsModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Induction Attendance")));



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

//Overall Tab on Officer Page
void mainWidget::on_offMenuButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void mainWidget::on_offAddStudentButton_clicked()
{
    //inserts a blank record with 6 columns to be edited
   QList<QStandardItem *> newRecord;
   for (int i = 0; i < currentStudentCols; i++)
   {
       newRecord.append(new QStandardItem(""));
   }
   currentStudentsModel->appendRow(newRecord);
}
