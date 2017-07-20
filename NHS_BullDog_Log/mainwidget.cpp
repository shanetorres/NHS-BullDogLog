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

    //instantiating the custom delegate
    currentStudentsDelegate = new officerDelegate(this);

    //connecting all delegate signals to desired slots on the mainwidget
    connect(currentStudentsDelegate, SIGNAL(studentNameEdited(CurrentStudent, int)), this, SLOT(on_studentNameEdited(CurrentStudent, int)));
    connect(currentStudentsDelegate, SIGNAL(studentSpinEdited(CurrentStudent, int)), this, SLOT(on_studentSpinEdited(CurrentStudent, int)));
    connect(currentStudentsDelegate, SIGNAL(studentComboEdited(CurrentStudent,int)), this, SLOT(on_studentComboEdited(CurrentStudent,int)));


    //creating the model for all current students and setting resizing parameters for the view
    currentStudentsModel = new QStandardItemModel(this);
    populateCurrentStudentsModel();          //reading data from file into the table
    ui->currentTableView->setModel(currentStudentsModel);
    ui->currentTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //assigning the custom delegate to the view
    ui->currentTableView->setItemDelegate(currentStudentsDelegate);

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


/*----OVERALL TAB DELEGATE SLOTS----*/


//assings data from line edits to an object in the vector based on the row number
void mainWidget::on_studentNameEdited(CurrentStudent student, int row)
{
    currentStudents[row].setFirstName(student.getFirstName());
    currentStudents[row].setLastName(student.getLastName());
    qDebug() << "Student Data: " << currentStudents[row].getFirstName() << ", " << currentStudents[row].getLastName();
    writeToFile();
}

//assings data from the spin boxes to an object in the vector
void mainWidget::on_studentSpinEdited(CurrentStudent student, int row)
{
    currentStudents[row].setContributions(student.getContributions());
    currentStudents[row].setServProjects(student.getServProjects());
    currentStudents[row].setAttendedMeetings(student.getAttendedMeetings());
    qDebug() << "Student Data AFTER SPIN: " << currentStudents[row].getFirstName() << ", " << currentStudents[row].getLastName() << ", "
             << currentStudents[row].getContributions() << ", " << currentStudents[row].getServProjects()
             << ", " << currentStudents[row].getAttendedMeetings() << ", " << currentStudents[row].getInductionAttendance();
    writeToFile();
}

//assings data from the combo box to an object in the vector
void mainWidget::on_studentComboEdited(CurrentStudent student, int row)
{
    currentStudents[row].setInductionAttendance(student.getInductionAttendance());
    qDebug() << "ATTENDANCE: " << currentStudents[row].getInductionAttendance();
    writeToFile();
}

/*----END OVERALL TAB DELEGATE SLOTS----*/

//writes all objects in the current student vector to a file
void mainWidget::writeToFile()
{
    QString filename = "currentstudents.csv";
    QFile file(filename);
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    QTextStream stream(&file);
    for (int i = 0; i < currentStudents.size(); i++)
    {
        //not the best solution, but eliminating a bug that sets these values to random numbers if not entered
        if (currentStudents[i].getContributions() > 100 | currentStudents[i].getContributions() < 0)
        {
            currentStudents[i].setContributions(0);
        }
        if (currentStudents[i].getServProjects() > 100 | currentStudents[i].getServProjects() < 0)
        {
            currentStudents[i].setServProjects(0);
        }

        stream << currentStudents[i].getFirstName() << "," << currentStudents[i].getLastName() << ","
                                                   << currentStudents[i].getContributions() << "," << currentStudents[i].getServProjects()
                                                   << "," << currentStudents[i].getAttendedMeetings() << "," << currentStudents[i].getInductionAttendance() << "," << endl;
    }
}


//populating the current students table with information from the data file
void mainWidget::populateCurrentStudentsModel()
{
    /*ISSUES:
        - need a way to not let certain input from the file be uploaded into the table, i.e. a string into a spinbox
        - need a try catch block when a new student is passed into the vector as to prevent an out of bounds error
    */

    QString filename = "currentstudents.csv";
    QFile file(filename);
    file.open(QIODevice::ReadOnly);

        int lineindex = 0;                     // file line counter
        QTextStream input(&file);                 // read to text stream
    if (file.size() < 10)
    {
        qDebug() << "FILE IS EMPTY";

    }
    else
    {
        while (!input.atEnd()) {
        QVector<QString> newRecord;
            // read one line from textstream(separated by "\n")
            QString fileLine = input.readLine();

            // parse the read line into separate pieces(tokens) with "," as the delimiter
            QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);

            // load parsed data to model accordingly
            for (int i = 0; i < lineToken.size(); i++) {

                QString value = lineToken.at(i);
                newRecord.push_back(value);
                QStandardItem *item = new QStandardItem(value);
                currentStudentsModel->setItem(lineindex, i, item);
            }
            //creating a student object with the information parsed from the file
            CurrentStudent student(newRecord[0], newRecord[1], newRecord[2].toInt(), newRecord[3].toInt(), newRecord[4].toInt(), newRecord[5].toUInt());
            currentStudents.push_back(student);
            qDebug() << "POPULATE STUDENT: " << student.getFirstName() << ", " << student.getLastName() << ", " << student.getContributions()
                     << ", " << student.getServProjects() << ", " << student.getAttendedMeetings() << ", " << student.getInductionAttendance();
            lineindex++;
        }
}


}




/*-------------------END OVERALL TAB ON OFFICER RECORDS-------------------*/
