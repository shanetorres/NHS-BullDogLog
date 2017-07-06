#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);

    /*ADMIN RECORDS*/




    /*OFFICER RECORDS*/

    //creating the model for all current students and setting resizing parameters for the view
    currentStudentsDelegate = new officerDelegate(this);
    currentStudentsModel = new QStandardItemModel(this);
    ui->currentTableView->setModel(currentStudentsModel);
    ui->currentTableView->setItemDelegate(currentStudentsDelegate);
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

void mainWidget::on_adminButton_clicked() { ui->stackedWidget->setCurrentIndex(1); }

void mainWidget::on_officerButton_clicked() { ui->stackedWidget->setCurrentIndex(2); }


void mainWidget::on_quitButton_clicked() { QApplication::quit(); }

//OVERALL TAB ON OFFICER PAGE
void mainWidget::on_offMenuButton_clicked() { ui->stackedWidget->setCurrentIndex(0); }

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

void mainWidget::on_offDeleteStudentButton_clicked()
{
    ui->offDeleteStudentButton->setEnabled(false);
    ui->offMenuButton->setEnabled(false);
    ui->offAddStudentButton->setEnabled(false);

    //Message box confirms whether or not the record should be deleted
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete Student",
                 "Are you sure you want to delete this student?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        officerDeleteRecord();
    }
    else {
        enableButtons();
    }
}

//deletes the selected record from the view
void mainWidget::officerDeleteRecord()
{
    currentStudentsModel->removeRows(ui->currentTableView->currentIndex().row(),1);
    enableButtons();
}

//enables buttons on officer page for clicking
void mainWidget::enableButtons()
{
    ui->offDeleteStudentButton->setEnabled(true);
    ui->offMenuButton->setEnabled(true);
    ui->offAddStudentButton->setEnabled(true);
}
