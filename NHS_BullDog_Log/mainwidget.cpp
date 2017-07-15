#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <qDebug>
#include <iostream>
#include <QModelIndex>

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);

    /*----------------------------ADMIN RECORDS------------------------------*/




    /*----------------------------OFFICER RECORDS----------------------------*/

    /*<OVERALL PAGE>*/

    //instantiating the custom delegate and connecting a signal to the studentEdited slot to allow for data to be transferred
    currentStudentsDelegate = new officerDelegate(this);
    spinDelegate = new OfficerSpinDelegate(this);
    comboDelegate =new OfficerComboDelegate(this);

    //connecting all delegates to desired slots on the mainwidget
    connect(currentStudentsDelegate, SIGNAL(studentNameEdited(CurrentStudent, int)), this, SLOT(on_studentNameEdited(CurrentStudent, int)));
    connect(spinDelegate, SIGNAL(studentSpinBoxEdited(CurrentStudent, int)), this, SLOT(on_studentSpinEdited(CurrentStudent, int)));
    connect(comboDelegate, SIGNAL(studentComboBoxEdited(CurrentStudent,int)), this, SLOT(on_studentComboEdited(CurrentStudent,int)));

    //creating the model for all current students and setting resizing parameters for the view
    currentStudentsModel = new QStandardItemModel(this);
    ui->currentTableView->setModel(currentStudentsModel);
    ui->currentTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //assigning delegates to specific columns
    ui->currentTableView->setItemDelegateForColumn(0, currentStudentsDelegate);
    ui->currentTableView->setItemDelegateForColumn(1, currentStudentsDelegate);
    for (int i = 2; i < 5; i++)
    {
        ui->currentTableView->setItemDelegateForColumn(i, spinDelegate);
    }
    ui->currentTableView->setItemDelegateForColumn(5, comboDelegate);



    //setting the header text of the model
    currentStudentsModel->setHorizontalHeaderItem(0, new QStandardItem(QString("First Name")));
    currentStudentsModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Last Name")));
    currentStudentsModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Contributions")));
    currentStudentsModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Service Projects")));
    currentStudentsModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Meetings Attended")));
    currentStudentsModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Induction Attendance")));

    /*<END OVERALL PAGE>*/

}

mainWidget::~mainWidget()
{
    delete ui;
}

void mainWidget::on_adminButton_clicked() { ui->stackedWidget->setCurrentIndex(1); }

void mainWidget::on_officerButton_clicked() { ui->stackedWidget->setCurrentIndex(2); }

void mainWidget::on_quitButton_clicked() { QApplication::quit(); }



/*---------------------OVERALL TAB ON OFFICER PAGE--------------------*/

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

   //creating a currentstudent object and pushing it into a vector everytime a row is added
   CurrentStudent student;
   currentStudents.push_back(student);


   totalStudents++;
   for (int i = 0; i < currentStudents.size(); i++)
   {
       qDebug() << "STUDENT " << i << "NAME: " << currentStudents[i].getFirstName();
   }
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

//deletes the selected record from the view as well as from the vector
void mainWidget::officerDeleteRecord()
{
    //removes current student from vector
    if (totalStudents != 0)
    {
        currentStudents.erase(currentStudents.begin()+ui->currentTableView->currentIndex().row());
        totalStudents--;
    }
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

/*DELEGATE SLOTS FROM OVERALL PAGE ON OFFICER RECORDS*/

//this slot assigns the emitted student to a student in the currentStudents vector
void mainWidget::on_studentNameEdited(CurrentStudent student, int row)
{
    //these statements set the data for an object in the vector using the values from the delegate and the row as the index
    currentStudents[row].setFirstName(student.getFirstName());
    currentStudents[row].setLastName(student.getLastName());


    qDebug() << "Student Data: " << currentStudents[row].getFirstName() << ", " << currentStudents[row].getLastName();
}

//saves the data entered in the spinboxes into the currentStudents vector
void mainWidget::on_studentSpinEdited(CurrentStudent student, int row)
{
    currentStudents[row].setContributions(student.getContributions());
    currentStudents[row].setServProjects(student.getServProjects());
    currentStudents[row].setAttendedMeetings(student.getAttendedMeetings());

     qDebug() << "Student Data AFTER SPIN: " << currentStudents[row].getFirstName() << ", " << currentStudents[row].getLastName()
              << currentStudents[row].getContributions() << ", " << currentStudents[row].getServProjects()
              << ", " << currentStudents[row].getAttendedMeetings() << " " << currentStudents[row].getInductionAttendance();
}

//saves the data entered in the combo  box into the currentStudents vector
void mainWidget::on_studentComboEdited(CurrentStudent student, int row)
{
    currentStudents[row].setInductionAttendance(student.getInductionAttendance());
}

/*END OFFICER OVERALL DELEGATE SLOTS*/

/*-------------------END OVERALL TAB ON OFFICER RECORDS-------------------*/
