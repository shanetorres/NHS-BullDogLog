#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <qDebug>
#include <iostream>
#include <QModelIndex>
#include <stdexcept>

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);

    /*----------------------------ADMIN RECORDS------------------------------*/


    //YO GIT SUCKS


    /*----------------------------OFFICER RECORDS----------------------------*/

    /*ISSUES:
            - need validation test or try catch block to eliminate problematic records when reading from a file
            - need a way to enable buttons if the close (x) button is clicked on the add event dialog
            - events need to be deleted from every students event vector when the delete event button is clicked
    */

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

    /*<CONTRIBUTIONS PAGE>*/

    contributionsModel = new QStandardItemModel(this);
    addDialog = new AddContributionDialog(this);
    contributionDelegate = new ContributionDelegate(this);

    //connecting the edited signal from the add event dialog box to the main widget
    connect(addDialog, SIGNAL(eventNameEdited(QString)), this, SLOT(on_eventAdded(QString)));
    connect(addDialog, SIGNAL(cancelClicked()), this, SLOT(on_cancelClicked()));
    connect(contributionDelegate, SIGNAL(eventEdited(QString,int,int)), this, SLOT(on_eventEdited(QString,int,int)));



    ui->contributionsTableView->setModel(contributionsModel);
    initializeContModel();              //sets the header text for the first two columns as well as resizing properties
    updateContributionsModel();
    ui->contributionsTableView->setItemDelegate(contributionDelegate);

    //manual column counter
    contCols = 2;

    /*<END CONTRIBUTIONS PAGE>*/

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
       newRecord.append(new QStandardItem(" "));
   }
   currentStudentsModel->appendRow(newRecord);

   //creating a currentstudent object and pushing it into a vector everytime a row is added
   CurrentStudent student;
   for (int j = 0; j < contributionsModel->columnCount()-2; j++)
   {
       student.setStudentEvent(" ");
   }
   currentStudents.push_back(student);


   totalStudents++;
   for (int i = 0; i < currentStudents.size(); i++)
   {
       qDebug() << "STUDENT " << i << "NAME: " << currentStudents[i].getFirstName();
   }


}

void mainWidget::on_offDeleteStudentButton_clicked()
{
    disableButtons();

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
    writeToFile();
    updateContributionsModel();
}

//enables buttons on officer page for clicking
void mainWidget::enableButtons()
{
    ui->offDeleteStudentButton->setEnabled(true);
    ui->offMenuButton->setEnabled(true);
    ui->offAddStudentButton->setEnabled(true);
    ui->contAddEventButton->setEnabled(true);
    ui->contDeleteEventButton->setEnabled(true);
    ui->contMenuButton->setEnabled(true);
}

void mainWidget::disableButtons()
{
    ui->offDeleteStudentButton->setEnabled(false);
    ui->offMenuButton->setEnabled(false);
    ui->offAddStudentButton->setEnabled(false);
    ui->contAddEventButton->setEnabled(false);
    ui->contDeleteEventButton->setEnabled(false);
    ui->contMenuButton->setEnabled(false);
}


/*----OVERALL TAB DELEGATE SLOTS----*/


//assings data from line edits to an object in the vector based on the row number
void mainWidget::on_studentNameEdited(CurrentStudent student, int row)
{
    currentStudents[row].setFirstName(student.getFirstName());
    currentStudents[row].setLastName(student.getLastName());
    qDebug() << "Student Data: " << currentStudents[row].getFirstName() << ", " << currentStudents[row].getLastName();
    updateContributionsModel();

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
        if (currentStudents[i].getContributions() > 100 || currentStudents[i].getContributions() < 0)
        {
            currentStudents[i].setContributions(0);
        }
        if (currentStudents[i].getServProjects() > 100 || currentStudents[i].getServProjects() < 0)
        {
            currentStudents[i].setServProjects(0);
        }
        if (currentStudents[i].getAttendedMeetings() > 100 || currentStudents[i].getAttendedMeetings() < 0)
        {
            currentStudents[i].setAttendedMeetings(0);
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
        std::vector<QString> newRecord;
            // read one line from textstream(separated by "\n")
            QString fileLine = input.readLine();

            // parse the read line into separate pieces(tokens) with "," as the delimiter
            QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);

            // load parsed data to model accordingly
            try {
            for (int i = 0; i < lineToken.size(); i++) {
                QString value = lineToken.at(i);
                newRecord.push_back(value);
                QStandardItem *item = new QStandardItem(value);
                currentStudentsModel->setItem(lineindex, i, item);
            }


                //creating a student object with the information parsed from the file

                CurrentStudent student(newRecord.at(0), newRecord.at(1), newRecord.at(2).toInt(), newRecord.at(3).toInt(), newRecord.at(4).toInt(), newRecord.at(5).toUInt());
                currentStudents.push_back(student);

                //converting the boolean value of induction attendance to a string of text displayed to the user
                if (student.getInductionAttendance() == 0)
                {
                    QStandardItem *item = new QStandardItem("No");
                    currentStudentsModel->setItem(lineindex, 5, item);
                }
                else if (student.getInductionAttendance() == 1)
                {
                    QStandardItem *item = new QStandardItem("Yes");
                    currentStudentsModel->setItem(lineindex, 5, item);
                }

                qDebug() << "POPULATE STUDENT: " << student.getFirstName() << ", " << student.getLastName() << ", " << student.getContributions()
                                                        << ", " << student.getServProjects() << ", " << student.getAttendedMeetings() << ", " << student.getInductionAttendance();
             }
             catch (const std::out_of_range& e)
             {
                    qDebug() << "OUT OF RANGE ERROR: " << e.what();
             }

            lineindex++;
        }
    }
}


/*-------------------END OVERALL TAB ON OFFICER RECORDS-------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*------------------CONTRIBUTIONS TAB ON OFFICER RECORDS------------------*/

void mainWidget::initializeContModel()
{
    ui->contributionsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //setting the header text of the contributions model
    contributionsModel->setHorizontalHeaderItem(0, new QStandardItem(QString("First Name")));
    contributionsModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Last Name")));

}

void mainWidget::on_contMenuButton_clicked() { ui->stackedWidget->setCurrentIndex(0); }

void mainWidget::on_contAddEventButton_clicked()
{
    disableButtons();
    //the addDialog prompts the user to enter a name for a custom event
    addDialog->show();
}

//closes the dialog box
void mainWidget::on_cancelClicked()
{
    addDialog->close();
    enableButtons();
}

//slot that detects when an edited signal is emitted from the dialog box
void mainWidget::on_eventAdded(QString eventName)
{
    QList<QStandardItem *> newRecord;
    eventNames.push_back(eventName);
    addDialog->close();
    enableButtons();

    //this loop adds a blank event to every student in the vector
    for (int i = 0; i < currentStudents.size(); i++)
    {
        QString newEvent = " ";
        currentStudents[i].setStudentEvent(newEvent);
        newRecord.append(new QStandardItem(""));
    }

    //the blank event column is appended to the model
    contributionsModel->appendColumn(newRecord);
    contributionsModel->setHorizontalHeaderItem(eventNames.size() + 1, new QStandardItem(eventName));
    contCols++;


//    writeToContributionsFile();
}

//deletes the selected column
void mainWidget::on_contDeleteEventButton_clicked()
{
    qDebug() << "Before " << eventNames.size();
    if (ui->contributionsTableView->currentIndex().column() != 0 && ui->contributionsTableView->currentIndex().column() != 1 )
    {
    contributionsModel->removeColumns(ui->contributionsTableView->currentIndex().column(),1);
    contCols--;
    //*ISSUE*: needs code to remove the event from every students event vector
    eventNames.erase(eventNames.begin()+ui->contributionsTableView->currentIndex().column());
    qDebug() << "After" << eventNames.size();
    writeToContributionsFile();
    }
}

//slot receives a signal from the delegate everytime an event is edited
void mainWidget::on_eventEdited(QString event, int row, int column)
{
    qDebug() << "Student: " << currentStudents[row].getFirstName() << currentStudents[row].getLastName();
    QVector<QString> events;
    //the current student vector is copied to the events vector so it can be updated with the edit
    for (int i = 0; i < (contributionsModel->columnCount() - 2); i++)
    {
        events = currentStudents[row].getEventVector();
    }
    events[column - 2] = event;             //the edited event is added to the events vector at the index of the column
    currentStudents[row].setEventVector(events);        //copy method is called that updates the student vector with the events vector

    writeToContributionsFile();
}


//populates the contributions model with data from the overall tab everytime it is updated
void mainWidget::updateContributionsModel()
{
    qDebug() << "MOFO SIZE" << currentStudents.size();
    contributionsModel->clear();
    initializeContModel();
    for (int i = 0; i < currentStudents.size(); i++)
    {
        QList<QStandardItem*> fullName;
        fullName.append(new QStandardItem(currentStudents[i].getFirstName()));
        fullName.append(new QStandardItem(currentStudents[i].getLastName()));

        for (int j = 0; j < eventNames.size(); j++)
        {
            contributionsModel->setHorizontalHeaderItem(j+2, new QStandardItem(eventNames[j])); //adding all the event names to the header
            fullName.append(new QStandardItem(currentStudents[i].getStudentEvent(j)));
        }

        contributionsModel->appendRow(fullName);
    }

}

//writes the event names as well as each student's contribution to a file
void mainWidget::writeToContributionsFile()
{
    qDebug() << "EVENT SIZE" << eventNames.size() << currentStudents.size();
    QString filename = "contributions.csv";
    QFile file(filename);
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    QTextStream stream(&file);
//    for (int k = 0; k < eventNames.size(); k++)
//    {
//        qDebug() << "YO EVENT #" << k << ": " << currentStudents[0].getStudentEvent(k) << ",";
//    }
    for (int i = 0; i < eventNames.size(); i++)
    {
        stream << eventNames[i] << ",";
    }
    stream << endl;
    for (int j = 0; j < currentStudents.size(); j++)
    {
        for (int k = 0; k < eventNames.size(); k++)
        {
            stream << currentStudents[j].getStudentEvent(k) << ",";
        }
        stream << endl;
    }
}

//NOT WORKING
//populates the contributions tab with current students names and event data from contributions.csv
void mainWidget::populateContributionsModel()
{
    contributionsModel->clear();
    initializeContModel();
    //sets the data for the first two columns to each student's first and last name
    for (int i = 0; i < currentStudents.size(); i++)
    {
        QList<QStandardItem*> fullName;
        fullName.append(new QStandardItem(currentStudents[i].getFirstName()));
        fullName.append(new QStandardItem(currentStudents[i].getLastName()));
        contributionsModel->appendRow(fullName);
    }

    QString filename = "contributions.csv";
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    int lineindex = 0;
    QTextStream input(&file);
    QVector<QString> values;

    while (!input.atEnd())
    {
        QString fileLine = input.readLine();
        QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);

        if (lineindex == 0)             //gets the first line of the file, which is the names of the e evnts
        {
            for (int i = 0; i < lineToken.size(); i++)
            {
                QString value = lineToken.at(i);
                eventNames.push_back(value);

                contributionsModel->setHorizontalHeaderItem(i+2, new QStandardItem(value));

                qDebug() << "EVENT NAMES: " << eventNames[i];

             }

        lineindex++;
        }
        else if(lineindex > 0)          //gets every other line afterwards, which is the student event data
        {
            for (int j = 0; j < lineToken.size(); j++)
            {
                QString value = lineToken.at(j);
//                values.push_back(value);
                contributionsModel->setItem(lineindex - 1, j + 2, new QStandardItem(value));

                currentStudents[lineindex-1].setStudentEvent(value);

            }

//                            QString value = lineToken.at(0);
//                            values.push_back(value);
//            currentStudents[0].setStudentEvent(value);
//            contributionsModel->setItem(0, 2, new QStandardItem(value));
            lineindex++;
        }

    }
}

