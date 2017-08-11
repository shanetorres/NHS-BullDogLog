#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <qDebug>
#include <iostream>
#include <QModelIndex>
#include <stdexcept>
#include <QStandardItem>
#include <QPushButton>
#include <QSizePolicy>

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);

     /*----------------------------GRAPHICS------------------------------*/

    this->setWindowTitle("NHS Bulldog Log");
    this->setWindowIcon(QIcon(":/nhslogo_a2I_icon.ico"));

    //this->setStyleSheet("background-color: blue;");
    //ui->groupBox_2->setStyleSheet("background-color: white; border-color: black;");
    //ui->currentTableView_2->setStyleSheet("background-color: white; border-color: black;");

    QImage image(":/nhslogo.png");
    QImage image2 = image.scaled(400, 600, Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(image2));

    /*----------------------------ADMIN RECORDS------------------------------*/

    //instantiating the custom delegate
    currentAdminDelegate = new adminDelegate(this);

    //connecting all delegate signals to desired slots on the mainwidget
    connect(currentAdminDelegate, SIGNAL(studentNameEdited_2(ProspectStudent, int)), this, SLOT(on_studentNameEdited_2(ProspectStudent, int)));
    connect(currentAdminDelegate, SIGNAL(studentComboEdited_2(ProspectStudent,int)), this, SLOT(on_studentComboEdited_2(ProspectStudent,int)));
    connect(currentAdminDelegate, SIGNAL(studentClassEdited(ProspectStudent,int)), this, SLOT(on_studentClassEdited(ProspectStudent,int)));
    connect(currentAdminDelegate, SIGNAL(studentStatusEdited(ProspectStudent,int)), this, SLOT(on_studentStatusEdited(ProspectStudent,int)));
    //connect(currentAdminDelegate, SIGNAL(studentNotesEdited(ProspectStudent,int)), this, SLOT(on_studentNotesEdited(ProspectStudent,int)));


    //creating the model for all current students and setting resizing parameters for the view
    currentAdminModel = new QStandardItemModel(this);
    populateCurrentProspectStudentsModel();
    ui->currentTableView_2->setModel(currentAdminModel);
    ui->currentTableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //assigning the custom delegate to the view
    ui->currentTableView_2->setItemDelegate(currentAdminDelegate);

    //setting the header text of the model
    currentAdminModel->setHorizontalHeaderItem(0, new QStandardItem(QString("First Name")));
    currentAdminModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Last Name")));
    currentAdminModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Application")));
    currentAdminModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Essay")));
    currentAdminModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Teacher Recommendations")));
    currentAdminModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Board Approval")));
    currentAdminModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Gpa")));
    currentAdminModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Class")));
    currentAdminModel->setHorizontalHeaderItem(8, new QStandardItem(QString("Status")));
    currentAdminModel->setHorizontalHeaderItem(9, new QStandardItem(QString("Notes")));

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
    populateContributionsModel();
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

void mainWidget::on_offMenuButton_clicked() { ui->stackedWidget->setCurrentIndex(0); mainWidget::setFixedSize(850, 550); }

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
    writeToContributionsFile();

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
        if (currentStudents[i].getInductionAttendance() > 1)
        {
            currentStudents[i].setInductionAttendance(false);
        }

        stream << currentStudents[i].getFirstName() << "," << currentStudents[i].getLastName() << ","
                                                   << currentStudents[i].getContributions() << "," << currentStudents[i].getServProjects()
                                                   << "," << currentStudents[i].getAttendedMeetings() << "," << currentStudents[i].getInductionAttendance() << "," << endl;
    }
}


//populating the current students table with information from the data file
void mainWidget::populateCurrentStudentsModel()
{
    QString filename = "currentstudents.csv";
    QFile file(filename);

    if(file.open(QIODevice::ReadOnly))
    {
        int lineindex = 0;                     // file line counter
        QTextStream input(&file);                 // read to text stream
        if (file.size() < 10)
        {
            qDebug() << "FILE IS EMPTY";

        }
        else
        {
            while (!input.atEnd())
            {
                std::vector<QString> newRecord;
                // read one line from textstream(separated by "\n")
                QString fileLine = input.readLine();

                // parse the read line into separate pieces(tokens) with "," as the delimiter
                QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);

                // load parsed data to model accordingly
                try {
                    for (int i = 0; i < lineToken.size(); i++)
                    {
                        QString value = lineToken.at(i);
                        newRecord.push_back(value);
                        QStandardItem *item = new QStandardItem(value);
                        currentStudentsModel->setItem(lineindex, i, item);
                    }
                    //this whole block of if statements is a validation test that eliminates faulty records that could cause out of bounds errors
                    if (newRecord.size() > 0)
                    {

                        if (newRecord[0].size() == 0 || newRecord[0] == "0" || (newRecord[0] >= 48 && newRecord[0] <= 57))
                        {
                            qDebug() << "IN";
                            newRecord.clear();
                            newRecord.push_back(" ");
                            newRecord.push_back(" ");
                            newRecord.push_back("0");
                            newRecord.push_back("0");
                            newRecord.push_back("0");
                            newRecord.push_back("No");
                            for (int j = 0; j < lineToken.size(); j++)
                            {
                                currentStudentsModel->setItem(lineindex, j, new QStandardItem(newRecord[j]));
                            }

                        }
                        if (newRecord[1].size() == 0 || newRecord[1] == "0" || (newRecord[1] >= 48 && newRecord[1] <= 57))
                        {
                            qDebug() << "IN 2";
                            newRecord.clear();
                            newRecord.push_back(" ");
                            newRecord.push_back(" ");
                            newRecord.push_back("0");
                            newRecord.push_back("0");
                            newRecord.push_back("0");
                            newRecord.push_back("No");
                            for (int j = 0; j < lineToken.size(); j++)
                            {
                                currentStudentsModel->setItem(lineindex, j, new QStandardItem(newRecord[j]));
                            }
                        }
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

    writeToContributionsFile();
}

//deletes the selected column
void mainWidget::on_contDeleteEventButton_clicked()
{
    if (ui->contributionsTableView->currentIndex().column() != 0 && ui->contributionsTableView->currentIndex().column() != 1 )
    {
    contributionsModel->removeColumns(ui->contributionsTableView->currentIndex().column(),1);
    contCols--;
    eventNames.erase(eventNames.begin()+ui->contributionsTableView->currentIndex().column() - 1);   //removes the name of the deleted event from the names vector
    //copies the current currentStudents event vector into a temporary one, deletes the specified event, and replaces the currentStudents vector with the updated one
    for (int i = 0; i < currentStudents.size(); i++)
    {
        QVector<QString> tempEvents = currentStudents[i].getEventVector();
        tempEvents.erase(tempEvents.begin()+ui->contributionsTableView->currentIndex().column() - 1);
        currentStudents[i].setEventVector(tempEvents);
    }
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
    contributionsModel->clear();
    initializeContModel();

    for (int i = 0; i < currentStudents.size(); i++)
    {
        QList<QStandardItem*> fullName;
        QStandardItem* first = new QStandardItem(currentStudents[i].getFirstName());
        first->setFlags(first->flags() & ~Qt::ItemIsEditable);  //changing the item flags to make the student name non editable from the contributions page
        QStandardItem* last = new QStandardItem(currentStudents[i].getLastName());
        last->setFlags(last->flags() & ~Qt::ItemIsEditable);
        fullName.append(first);
        fullName.append(last);

        for (int j = 0; j < eventNames.size(); j++)
        {
            contributionsModel->setHorizontalHeaderItem(j+2, new QStandardItem(eventNames[j])); //adding all the event names to the header
            fullName.append(new QStandardItem(currentStudents[i].getStudentEvent(j)));
        }

        contributionsModel->appendRow(fullName);
    }
    writeToContributionsFile();
}

//writes the event names as well as each student's contribution to a file
void mainWidget::writeToContributionsFile()
{
    qDebug() << "EVENT SIZE" << eventNames.size() << currentStudents.size();
    QString filename = "contributions.csv";
    QFile file(filename);
    if(file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream(&file);

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
}


//populates the contributions tab with current students names and event data from contributions.csv
void mainWidget::populateContributionsModel()
{
    contributionsModel->clear();
    initializeContModel();
    //sets the data for the first two columns to each student's first and last name
    for (int i = 0; i < currentStudents.size(); i++)
    {
        QList<QStandardItem*> fullName;
        QStandardItem* first = new QStandardItem(currentStudents[i].getFirstName());
        first->setFlags(first->flags() & ~Qt::ItemIsEditable);      //changing the item flags to make the student name non editable from the contributions page
        QStandardItem* last = new QStandardItem(currentStudents[i].getLastName());
        last->setFlags(last->flags() & ~Qt::ItemIsEditable);
        fullName.append(first);
        fullName.append(last);
        contributionsModel->appendRow(fullName);
    }

    QString filename = "contributions.csv";
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly))
    {
        int lineindex = 0;
        QTextStream input(&file);
        QVector<QString> values;

        while (!input.atEnd())
        {
            QString fileLine = input.readLine();
            QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);

            if (lineindex == 0)             //gets the first line of the file, which is the names of the events
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
            else if(lineindex > 0 && lineindex <= currentStudents.size())          //gets every other line afterwards, which is the student event data
            {
                for (int j = 0; j < lineToken.size(); j++)
                {
                QString value = lineToken.at(j);
                contributionsModel->setItem(lineindex - 1, j + 2, new QStandardItem(value));
                currentStudents[lineindex-1].setStudentEvent(value);
                }
                lineindex++;
            }
        }
    }
}







/*~~~~~~~~~~~~~~~~~~~ADMIN RECORDS BEGIN~~~~~~~~~~~~~~~~~*/

void mainWidget::on_offMenuButton_2_clicked() { ui->stackedWidget->setCurrentIndex(0); mainWidget::setFixedSize(850, 550); }

void mainWidget::on_offAddStudentButton_2_clicked()
{
    //inserts a blank record with 9 columns to be edited
   QList<QStandardItem *> newRecord;
   for (int i = 0; i < currentAdminCols; i++)
   {
     if (i == 0 || i ==1)
     {
     newRecord.append(new QStandardItem(" "));
     }
     else if (i == 2 || i == 3 || i == 4 || i == 5 || i == 6)
     {
     newRecord.append(new QStandardItem("No"));
     }
     else if (i == 7)
     {
     newRecord.append(new QStandardItem(" "));
     }
     else if (i == 8)
     {
     newRecord.append(new QStandardItem("Applicant"));
     }
     else if (i == 9)
     {
     newRecord.append(new QStandardItem("Notes"));
     }
   }
   currentAdminModel->appendRow(newRecord);

   ProspectStudent student;
   currentProspectStudents.push_back(student);

   totalProspectStudents++;
}

void mainWidget::on_offDeleteStudentButton_2_clicked()
{
    disableButtons();

    //Message box confirms whether or not the record should be deleted
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete Student",
                 "Are you sure you want to delete this student?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        adminDeleteRecord();
    }
    else {
        enableButtons();
    }
}

//deletes the selected record from the view as well as from the vector
void mainWidget::adminDeleteRecord()
{
    //removes current student from vector
    if (totalProspectStudents != 0)
    {
        currentProspectStudents.erase(currentProspectStudents.begin()+ui->currentTableView_2->currentIndex().row());
        totalStudents--;
    }
    currentAdminModel->removeRows(ui->currentTableView_2->currentIndex().row(),1);
    enableButtons();
    writeToAdminFile();
}

//assings data from line edits to an object in the vector based on the row number
void mainWidget::on_studentNameEdited_2(ProspectStudent student, int row)
{
    qDebug() << "IN";
    currentProspectStudents[row].setFirstName(student.getFirstName());
    currentProspectStudents[row].setLastName(student.getLastName());

    qDebug() << "Student Data: " << currentProspectStudents[row].getFirstName() << ", " << currentProspectStudents[row].getLastName();
    writeToAdminFile();

}

//assings data from the combo box to an object in the vector
void mainWidget::on_studentComboEdited_2(ProspectStudent student, int row)
{
    currentProspectStudents[row].setApplicationBool(student.getApplicationBool());
    currentProspectStudents[row].setEssayBool(student.getEssayBool());
    currentProspectStudents[row].setRecommendationBool(student.getRecommendationBool());
    currentProspectStudents[row].setApprovalBool(student.getApprovalBool());
    currentProspectStudents[row].setStudentGpa(student.getStudentGpa());

    qDebug() << "REQUIREMENTS: " << currentProspectStudents[row].getApplicationBool() << ", " << currentProspectStudents[row].getEssayBool() << ", "
             << currentProspectStudents[row].getRecommendationBool() << ", " << currentProspectStudents[row].getApprovalBool() << ", "
             << currentProspectStudents[row].getStudentGpa();
   writeToAdminFile();

}
//assings data from the spin boxes to an object in the vector
void mainWidget::on_studentClassEdited(ProspectStudent student, int row)
{
    currentProspectStudents[row].setStudentClass(student.getStudentClass());
    qDebug() << "Student Data AFTER SPIN: " << currentProspectStudents[row].getFirstName() << ", " << currentProspectStudents[row].getLastName() << ", "
             << currentProspectStudents[row].getStudentClass();
    writeToAdminFile();
}

void mainWidget::on_studentStatusEdited(ProspectStudent student, int row)
{
    currentProspectStudents[row].setStudentStatus(student.getStudentStatus());

    qDebug() << "Student Data: " << currentProspectStudents[row].getFirstName() << ", " << currentProspectStudents[row].getLastName() << ", "
             << currentProspectStudents[row].getStudentStatus();
    writeToAdminFile();

}



//writes all objects in the current prospect student vector to a file
void mainWidget::writeToAdminFile()
{

    QString filename = "currentprospectstudents.csv";
    QFile file(filename);
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    QTextStream stream(&file);

    for (int i = 0; i < currentProspectStudents.size(); i++) {
        if (currentProspectStudents[i].getApplicationBool() > 1)
        {
            currentProspectStudents[i].setApplicationBool(false);
        }
        if (currentProspectStudents[i].getEssayBool() > 1)
        {
            currentProspectStudents[i].setEssayBool(false);
        }
        if (currentProspectStudents[i].getRecommendationBool() > 1)
        {
            currentProspectStudents[i].setRecommendationBool(false);
        }
        if (currentProspectStudents[i].getApprovalBool() > 1)
        {
            currentProspectStudents[i].setApprovalBool(false);
        }
        if (currentProspectStudents[i].getStudentGpa() > 1)
        {
            currentProspectStudents[i].setStudentGpa(false);
        }
        if (currentProspectStudents[i].getStudentClass() > 100 || currentProspectStudents[i].getStudentClass() < 0)
        {
            currentProspectStudents[i].setStudentClass(11);
        }
        if (currentProspectStudents[i].getStudentStatus() > 1)
        {
            currentProspectStudents[i].setStudentStatus(false);
        }

        stream << currentProspectStudents[i].getFirstName() << "," << currentProspectStudents[i].getLastName() << ","
                                                   << currentProspectStudents[i].getApplicationBool() << "," << currentProspectStudents[i].getEssayBool() << ","
                                                   << currentProspectStudents[i].getRecommendationBool() << "," << currentProspectStudents[i].getApprovalBool() << ","
                                                   << currentProspectStudents[i].getStudentGpa() << "," << currentProspectStudents[i].getStudentClass() << ","
                                                   << currentProspectStudents[i].getStudentStatus() << "," << endl;
    }
}

//populating the current prospect students table with information from the data file
void mainWidget::populateCurrentProspectStudentsModel()
{
    QString filename = "currentprospectstudents.csv";
    QFile file(filename);

    if(file.open(QIODevice::ReadOnly))
    {
        int lineindex = 0;                     // file line counter
        QTextStream input(&file);                 // read to text stream
        if (file.size() < 10)
        {
            qDebug() << "FILE IS EMPTY";

        }
        else
        {
            while (!input.atEnd())
            {
                std::vector<QString> newRecord;
                // read one line from textstream(separated by "\n")
                QString fileLine = input.readLine();

                // parse the read line into separate pieces(tokens) with "," as the delimiter
                QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);

                // load parsed data to model accordingly
                try {
                    for (int i = 0; i < lineToken.size(); i++)
                    {
                        QString value = lineToken.at(i);
                        newRecord.push_back(value);
                        QStandardItem *item = new QStandardItem(value);
                        currentAdminModel->setItem(lineindex, i, item);
                    }
                    //this whole block of if statements is a validation test that eliminates faulty records that could cause out of bounds errors
                    if (newRecord.size() > 0)
                    {

                        if (newRecord[0].size() == 0 || newRecord[0] == "0" || (newRecord[0] >= 48 && newRecord[0] <= 57))
                        {
                            qDebug() << "IN";
                            newRecord.push_back(" ");
                            newRecord.push_back(" ");
                            newRecord.push_back("No");
                            newRecord.push_back("No");
                            newRecord.push_back("No");
                            newRecord.push_back("No");
                            newRecord.push_back("No");
                            newRecord.push_back("11");
                            newRecord.push_back("Applicant");
                            for (int j = 0; j < lineToken.size(); j++)
                            {
                                currentAdminModel->setItem(lineindex, j, new QStandardItem(newRecord[j]));
                            }

                        }
                        if (newRecord[1].size() == 0 || newRecord[1] == "0" || (newRecord[1] >= 48 && newRecord[1] <= 57))
                        {
                            qDebug() << "IN 2";
                            newRecord.push_back(" ");
                            newRecord.push_back(" ");
                            newRecord.push_back("No");
                            newRecord.push_back("No");
                            newRecord.push_back("No");
                            newRecord.push_back("No");
                            newRecord.push_back("No");
                            newRecord.push_back("11");
                            newRecord.push_back("Applicant");
                            for (int j = 0; j < lineToken.size(); j++)
                            {
                                currentAdminModel->setItem(lineindex, j, new QStandardItem(newRecord[j]));
                            }
                        }
                      }

                  //creating a prospect student object with the information parsed from the file
                  ProspectStudent student(newRecord.at(0), newRecord.at(1), newRecord.at(2).toUInt(), newRecord.at(3).toUInt(), newRecord.at(4).toUInt(),
                                          newRecord.at(5).toUInt(), newRecord.at(6).toUInt(), newRecord.at(7).toInt(), newRecord.at(8).toUInt(), newRecord.at(9));
                  currentProspectStudents.push_back(student);

                //converting the boolean value of induction attendance to a string of text displayed to the user
                if (student.getApplicationBool() == 0)
                {
                    QStandardItem *item = new QStandardItem("No");
                    currentAdminModel->setItem(lineindex, 2, item);
                }
                else if (student.getApplicationBool() == 1)
                {
                    QStandardItem *item = new QStandardItem("Yes");
                    currentAdminModel->setItem(lineindex, 2, item);
                }
                if (student.getEssayBool() == 0)
                {
                    QStandardItem *item = new QStandardItem("No");
                    currentAdminModel->setItem(lineindex, 3, item);
                }
                else if (student.getEssayBool() == 1)
                {
                    QStandardItem *item = new QStandardItem("Yes");
                    currentAdminModel->setItem(lineindex, 3, item);
                }
                if (student.getRecommendationBool() == 0)
                {
                    QStandardItem *item = new QStandardItem("No");
                    currentAdminModel->setItem(lineindex, 4, item);
                }
                else if (student.getRecommendationBool() == 1)
                {
                    QStandardItem *item = new QStandardItem("Yes");
                    currentAdminModel->setItem(lineindex, 4, item);
                }
                if (student.getApprovalBool() == 0)
                {
                    QStandardItem *item = new QStandardItem("No");
                    currentAdminModel->setItem(lineindex, 5, item);
                }
                else if (student.getApprovalBool() == 1)
                {
                    QStandardItem *item = new QStandardItem("Yes");
                    currentAdminModel->setItem(lineindex, 5, item);
                }
                if (student.getStudentGpa() == 0)
                {
                    QStandardItem *item = new QStandardItem("No");
                    currentAdminModel->setItem(lineindex, 6, item);
                }
                else if (student.getStudentGpa() == 1)
                {
                    QStandardItem *item = new QStandardItem("Yes");
                    currentAdminModel->setItem(lineindex, 6, item);
                }
                if (student.getStudentStatus() == 0)
                {
                    QStandardItem *item = new QStandardItem("Applicant");
                    currentAdminModel->setItem(lineindex, 8, item);
                }
                else if (student.getStudentGpa() == 1)
                {
                    QStandardItem *item = new QStandardItem("Member");
                    currentAdminModel->setItem(lineindex, 8, item);
                }

                qDebug() << "POPULATE PROSPECT STUDENT: " << student.getFirstName() << ", " << student.getLastName() << ", " << student.getApplicationBool() << ", "
                         << student.getEssayBool() << ", " << student.getRecommendationBool() << ", " << student.getApprovalBool() << ", "
                         << student.getStudentGpa() << ", " << student.getStudentClass() << ", " << student.getStudentStatus();
             }
             catch (const std::out_of_range& e)
             {
                    qDebug() << "OUT OF RANGE ERROR: " << e.what();
             }

            lineindex++;
        }
    }
    }
}




