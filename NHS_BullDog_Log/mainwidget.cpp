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
#include <QSortFilterProxyModel>

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);

    //populates boolean array used to determine sort order for each model
    for (int i = 0; i < 4; i++)
    {
        sortOrder[i] = 0;
    }

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
    connect(currentAdminDelegate, SIGNAL(studentNameEdited2(ProspectStudent, int)), this, SLOT(on_studentNameEdited2(ProspectStudent, int)));
    connect(currentAdminDelegate, SIGNAL(studentComboEdited2(ProspectStudent,int)), this, SLOT(on_studentComboEdited2(ProspectStudent,int)));
    connect(currentAdminDelegate, SIGNAL(studentClassEdited(ProspectStudent,int)), this, SLOT(on_studentClassEdited(ProspectStudent,int)));
    connect(currentAdminDelegate, SIGNAL(studentStatusEdited(ProspectStudent,int)), this, SLOT(on_studentStatusEdited(ProspectStudent,int)));
    connect(currentAdminDelegate, SIGNAL(studentNotesEdited(ProspectStudent,int)), this, SLOT(on_studentNotesEdited(ProspectStudent,int)));


    //creating the model for all current students and setting resizing parameters for the view
    currentAdminModel = new QStandardItemModel(this);
    populateCurrentProspectStudentsModel();
    ui->currentTableView_2->setModel(currentAdminModel);
    ui->currentTableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //assigning the custom delegate to the view
    ui->currentTableView_2->setItemDelegate(currentAdminDelegate);

    prospectHeader = ui->currentTableView_2->horizontalHeader();
    connect(prospectHeader, SIGNAL(sectionClicked(int)), this, SLOT(on_sectionClicked(int)));

    //setting the header text of the model
    currentAdminModel->setHorizontalHeaderItem(0, new QStandardItem(QString("First Name")));
    currentAdminModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Last Name")));
    currentAdminModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Application")));
    currentAdminModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Essay")));
    currentAdminModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Teacher Recommendations")));
    currentAdminModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Board Approval")));
    currentAdminModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Gpa")));
    currentAdminModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Grade Level")));
    currentAdminModel->setHorizontalHeaderItem(8, new QStandardItem(QString("NHS Status")));
    currentAdminModel->setHorizontalHeaderItem(9, new QStandardItem(QString("Notes")));

    /*----------------------------OFFICER RECORDS----------------------------*/


    /*<OVERALL PAGE>*/

    //instantiating the custom delegate
    currentStudentsDelegate = new officerDelegate(this);

    //connecting all delegate signals to desired slots on the mainwidget
    connect(currentStudentsDelegate, SIGNAL(studentEdited(int)), this, SLOT(on_studentEdited(int)));

    //creating the model for all current students and setting resizing parameters for the view
    currentStudentsModel = new QStandardItemModel(this);
    populateCurrentStudentsModel();          //reading data from file into the table

    ui->currentTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->currentTableView->setModel(currentStudentsModel);

    ui->currentTableView->setItemDelegate(currentStudentsDelegate);
    currentTableHeader = ui->currentTableView->horizontalHeader();
    connect(currentTableHeader, SIGNAL(sectionClicked(int)), this, SLOT(on_sectionClicked(int)));

    //assigning the custom delegate to the view



    //setting the header text of the model
    currentStudentsModel->setHorizontalHeaderItem(0, new QStandardItem(QString("First Name")));
    currentStudentsModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Last Name")));
    currentStudentsModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Contributions")));
    currentStudentsModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Service Projects")));
    currentStudentsModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Meetings Attended")));
    currentStudentsModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Induction Attendance")));
    currentStudentsModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Grade Level")));


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

    contributionHeader = ui->contributionsTableView->horizontalHeader();
    connect(contributionHeader, SIGNAL(sectionClicked(int)), this, SLOT(on_sectionClicked(int)));

    /*<END CONTRIBUTIONS PAGE>*/

    /*<SERVICE PROJECTS PAGE>*/

    serviceModel = new QStandardItemModel(this);
    serviceDelegate = new ServiceDelegate(this);

    connect(serviceDelegate, SIGNAL(serveEventEdited(QString,int,int)), this, SLOT(on_serveEventEdited(QString,int,int)));

    ui->serviceTableView->setModel(serviceModel);
    ui->serviceTableView->setItemDelegate(serviceDelegate);
    initializeServiceModel();
    populateServiceModel();


    /*<END SERVICE PROJECTS PAGE>*/

    /*<MEETINGS PAGE>*/

    meetingsModel = new QStandardItemModel(this);
    meetingsDialog = new AddMeetingsDialog(this);
    meetingsDelegate = new MeetingsDelegate(this);

    connect(meetingsDialog, SIGNAL(dateAdded(QString)), this, SLOT(on_dateAdded(QString)));
    connect(meetingsDialog, SIGNAL(cancelClicked()),this, SLOT(on_cancelMeetingsButtonClicked()));
    connect(meetingsDelegate, SIGNAL(meetingComboEdited(bool,int,int)), this, SLOT(on_meetingComboEdited(bool,int,int)));

    ui->meetingsTableView->setModel(meetingsModel);
    ui->meetingsTableView->setItemDelegate(meetingsDelegate);
    populateMeetingsModel();

    /*<END MEETINGS PAGE>*/

}

mainWidget::~mainWidget()
{
    delete ui;
}

void mainWidget::on_adminButton_clicked() { ui->stackedWidget->setCurrentIndex(1); }

void mainWidget::on_officerButton_clicked() { ui->stackedWidget->setCurrentIndex(2); }

void mainWidget::on_optionsButton_clicked()
{

}

void mainWidget::on_quitButton_clicked() { QApplication::quit(); }

void mainWidget::updateModels(int row)
{
    //this mess of code updates the names in the other models, huge performance saver
    QStandardItem* firstC = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(row,0)).toString());
    firstC->setFlags(firstC->flags() & ~Qt::ItemIsEditable);
    QStandardItem* lastC = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(row,1)).toString());
    lastC->setFlags(lastC->flags() & ~Qt::ItemIsEditable);
    contributionsModel->setItem(row,0,firstC);
    contributionsModel->setItem(row,1,lastC);

    QStandardItem* firstS = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(row,0)).toString());
    firstS->setFlags(firstS->flags() & ~Qt::ItemIsEditable);
    QStandardItem* lastS = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(row,1)).toString());
    lastS->setFlags(lastS->flags() & ~Qt::ItemIsEditable);
    serviceModel->setItem(row,0, firstS);
    serviceModel->setItem(row,1,lastS);

    QStandardItem* firstM = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(row,0)).toString());
    firstM->setFlags(firstM->flags() & ~Qt::ItemIsEditable);
    QStandardItem* lastM = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(row,1)).toString());
    lastM->setFlags(lastM->flags() & ~Qt::ItemIsEditable);
    meetingsModel->setItem(row,0, firstM);
    meetingsModel->setItem(row,1,lastM);
}

/*---------------------OVERALL TAB ON OFFICER PAGE--------------------*/

void mainWidget::on_sectionClicked(int index)
{
//    if (ui->stackedWidget->currentIndex() == 1)
//    {
//        if (sortOrder[0] == 0)
//        {
//            currentAdminModel->sort(index,Qt::AscendingOrder);
//            sortOrder[0] = 1;
//        }
//        else if (sortOrder[0] == 1)
//        {
//            currentAdminModel->sort(index,Qt::DescendingOrder);
//            sortOrder[0] = 0;
//        }
//    }
//    if (ui->stackedWidget->currentIndex() == 2)
//    {
//        if (ui->tabWidget->currentIndex() == 0)
//        {
//            if (sortOrder[1] == 0)
//            {
                currentStudentsModel->setSortRole(Qt::EditRole);
                currentStudentsModel->sort(index,Qt::AscendingOrder);
                contributionsModel->sort(index,Qt::AscendingOrder);
                serviceModel->sort(index,Qt::AscendingOrder);
                meetingsModel->sort(index,Qt::AscendingOrder);
//                sortOrder[1] = 1;
                writeToFile();
                for (int i = 0; i < currentStudents.size(); i++)
                {
                    qDebug() << "SORT CHECk PLS: ";
                    qDebug() << currentStudents[i].getFirstName() << "," << currentStudents[i].getLastName() << ","
                                                               << currentStudents[i].getContributions() << "," << currentStudents[i].getServProjects()
                                                               << "," << currentStudents[i].getAttendedMeetings() << "," << currentStudents[i].getInductionAttendance() << "," <<
                                                                  currentStudents[i].getGradeLevel() << "," << endl;
                }
//                }
//            }
//            else if (sortOrder[1] == 1)
//            {
//                currentStudentsModel->sort(index,Qt::DescendingOrder);
//                sortOrder[1] = 0;
//                writeToFile();
//            }
//            writeToFile();
//        }
//        if (ui->tabWidget->currentIndex() == 1)
//        {
//            if (sortOrder[2] == 0)
//            {
//                contributionsModel->sort(index,Qt::AscendingOrder);
//                sortOrder[2] = 1;
//            }
//            if (sortOrder[2] == 1)
//            {
//                contributionsModel->sort(index,Qt::DescendingOrder);
//                sortOrder[2] = 0;
//            }
//            writeToContributionsFile();
//        }
//    }
}

void mainWidget::on_offMenuButton_clicked() { ui->stackedWidget->setCurrentIndex(0); mainWidget::setFixedSize(850, 550); }

void mainWidget::on_offAddStudentButton_clicked()
{
    //inserts a blank record with 6 columns to be edited
   QList<QStandardItem *> newRecord;
   for (int i = 0; i < currentStudentCols; i++)
   {
       if (i == 2 || i == 3 || i == 4)
       {
       QStandardItem* data = new QStandardItem(" ");
       data->setFlags(data->flags() & ~Qt::ItemIsEditable);
       newRecord.append(data);
       }
       else
       {
       newRecord.append(new QStandardItem(" "));
       }
   }
   currentStudentsModel->appendRow(newRecord);

   //creating a currentstudent object and pushing it into a vector everytime a row is added
   CurrentStudent student;
   for (int j = 0; j < contributionsModel->columnCount()-2; j++)
   {
       student.setStudentEvent(" ");
       student.setServeEvent(" ");

   }
   for (int k = 0; k < meetingsModel->columnCount() - 2; k++)
   {
       student.setMeeting(false);
   }
   currentStudents.push_back(student);


   totalStudents++;
   for (int i = 0; i < currentStudents.size(); i++)
   {
       qDebug() << "STUDENT " << i << "NAME: " << currentStudents[i].getFirstName();
   }
   updateModels(currentStudentsModel->rowCount()-1);
    writeToContributionsFile();
    writeToServiceFile();
    writeToMeetingsFile();
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
    int row = ui->currentTableView->currentIndex().row();
    //removes current student from vector
    if (totalStudents != 0)
    {
        currentStudents.erase(currentStudents.begin()+ui->currentTableView->currentIndex().row());
        totalStudents--;
    }
    currentStudentsModel->removeRows(ui->currentTableView->currentIndex().row(),1);
    enableButtons();
    writeToFile();
    updateContributionsModel(row);
    updateServiceModel(row);
    updateMeetingsModel(row);
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
    ui->serveAddEventButton->setEnabled(true);
    ui->serveDeleteEventButton->setEnabled(true);
    ui->serveMenuButton->setEnabled(true);
    ui->meetingMenuButton->setEnabled(true);
    ui->addMeetingButton->setEnabled(true);
    ui->deleteMeetingButton->setEnabled(true);
}

void mainWidget::disableButtons()
{
    ui->offDeleteStudentButton->setEnabled(false);
    ui->offMenuButton->setEnabled(false);
    ui->offAddStudentButton->setEnabled(false);
    ui->contAddEventButton->setEnabled(false);
    ui->contDeleteEventButton->setEnabled(false);
    ui->contMenuButton->setEnabled(false);
    ui->serveAddEventButton->setEnabled(false);
    ui->serveDeleteEventButton->setEnabled(false);
    ui->serveMenuButton->setEnabled(false);
    ui->meetingMenuButton->setEnabled(false);
    ui->addMeetingButton->setEnabled(false);
    ui->deleteMeetingButton->setEnabled(false);
}


/*----OVERALL TAB DELEGATE SLOTS----*/

void mainWidget::on_studentEdited(int row)
{
    updateModels(row);
    writeToFile();
}


/*----END OVERALL TAB DELEGATE SLOTS----*/

//writes all objects in the current student vector to a file
void mainWidget::writeToFile()
{
    QString filename = "currentstudents.csv";
    QFile file(filename);

    // [Collect model data to QString]
    QString textData;
    int rows = currentStudentsModel->rowCount();
    int columns = currentStudentsModel->columnCount();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {

                textData += currentStudentsModel->data(currentStudentsModel->index(i,j)).toString();
                qDebug() << "TEXT DATA: " << currentStudentsModel->data(currentStudentsModel->index(i,j)).toString();
                textData += ",";     // for .csv file format
        }
        textData += "\n";             // (optional: for new line segmentation)
    }

    // [Save to file] (header file <QFile> needed)
    // .csv

    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    qDebug() << "TEXT DATA: " << textData;
        QTextStream stream(&file);
        stream << textData;

        file.close();
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
                        if (i == 2|| i == 3 || i == 4)
                        {
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                        }
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
                            newRecord.push_back("11");
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
                            newRecord.push_back("11");
                            for (int j = 0; j < lineToken.size(); j++)
                            {
                                currentStudentsModel->setItem(lineindex, j, new QStandardItem(newRecord[j]));
                            }
                        }
                        for (int i = 2; i < 7; i++)
                        {
                            if (newRecord[i].size() == 0)
                            {
                                newRecord.at(i) = " ";
                            }
                        }
                      }


                  //creating a student object with the information parsed from the file
                  CurrentStudent student(newRecord.at(0), newRecord.at(1), newRecord.at(2).toInt(), newRecord.at(3).toInt(), newRecord.at(4).toInt(), newRecord.at(5).toUInt(), newRecord.at(6).toInt());
                  currentStudents.push_back(student);

                //converting the boolean value of induction attendance to a string of text displayed to the user
//                if (student.getInductionAttendance() == 0)
//                {
//                    QStandardItem *item = new QStandardItem("No");
//                    currentStudentsModel->setItem(lineindex, 5, item);
//                }
//                else if (student.getInductionAttendance() == 1)
//                {
//                    QStandardItem *item = new QStandardItem("Yes");
//                    currentStudentsModel->setItem(lineindex, 5, item);
//                }

                totalStudents++;
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
    if (eventName.size() > 0)
    {
    eventNames.push_back(eventName);
    }
    else
    {
        eventNames.push_back(" ");
    }
    addDialog->close();
    enableButtons();

    //this loop adds a blank event to every student in the vector
    for (int i = 0; i < currentStudents.size(); i++)
    {
        QString newEvent = " ";
        currentStudents[i].setStudentEvent(newEvent);
        currentStudents[i].setServeEvent(newEvent);
        newRecord.append(new QStandardItem(""));
    }

    //the blank event column is appended to the model
    contributionsModel->appendColumn(newRecord);
    contributionsModel->setHorizontalHeaderItem(eventNames.size() + 1, new QStandardItem(eventName));
    contCols++;
    serviceModel->appendColumn(newRecord);
    serviceModel->setHorizontalHeaderItem(eventNames.size() + 1, new QStandardItem(eventName));

    writeToContributionsFile();
    writeToServiceFile();
}

//deletes the selected column
void mainWidget::on_contDeleteEventButton_clicked()
{
    //Message box confirms whether or not the record should be deleted
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete Meeting",
                 "Are you sure you want to delete this event?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (ui->contributionsTableView->currentIndex().column() != 0 && ui->contributionsTableView->currentIndex().column() != 1 )
        {
            contributionsModel->removeColumns(ui->contributionsTableView->currentIndex().column(),1);
            serviceModel->removeColumns(ui->contributionsTableView->currentIndex().column()+1,1);
            contCols--;
            eventNames.erase(eventNames.begin()+ui->contributionsTableView->currentIndex().column() - 1);   //removes the name of the deleted event from the names vector
            //copies the current currentStudents event vector into a temporary one, deletes the specified event, and replaces the currentStudents vector with the updated one
            for (int i = 0; i < currentStudents.size(); i++)
            {
                QVector<QString> tempEvents = currentStudents[i].getEventVector();
                tempEvents.erase(tempEvents.begin()+ui->contributionsTableView->currentIndex().column() - 1);
                currentStudents[i].setEventVector(tempEvents);
            }
            for (int i = 0; i < currentStudents.size(); i++)
            {
                QVector<QString> tempEvents = currentStudents[i].getServeVector();
                tempEvents.erase(tempEvents.begin()+ui->contributionsTableView->currentIndex().column() - 1);
                currentStudents[i].setServeVector(tempEvents);
            }
            writeToContributionsFile();
            writeToServiceFile();
            }
        }
    else
    {
        enableButtons();
    }
}

//slot receives a signal from the delegate everytime an event is edited
void mainWidget::on_eventEdited(QString event, int row, int column)
{
    int contributionsCounter = 0;
    qDebug() << "Student: " << currentStudents[row].getFirstName() << currentStudents[row].getLastName();
    QVector<QString> events;
    //the current student vector is copied to the events vector so it can be updated with the edit
    for (int i = 0; i < (contributionsModel->columnCount() - 2); i++)
    {
        events = currentStudents[row].getEventVector();
    }
    events[column - 2] = event;             //the edited event is added to the events vector at the index of the column
    currentStudents[row].setEventVector(events);        //copy method is called that updates the student vector with the events vector

    for (int i = 0; i < (contributionsModel->columnCount() - 2); i++)
    {
        events = currentStudents[row].getEventVector();
    }
    //loops through every value in the vector to detect meetings where the student was present
    for (int k = 0; k < events.size(); k++)
    {
        if (events[k].size() > 1)
        {
            contributionsCounter++;
        }
    }
    //updates the students meetings counter
    currentStudents[row].setContributions(contributionsCounter);
    //converts this counter to a string
    QString contString = QString::number(contributionsCounter);
    QStandardItem* newValue = new QStandardItem(contString);
    newValue->setFlags(newValue->flags() & ~Qt::ItemIsEditable);    //makes the item non editable
    currentStudentsModel->setItem(row, 2, newValue);      //updates the edited column in the current students view
    writeToFile();
    writeToContributionsFile();
}


//populates the contributions model with data from the overall tab everytime it is updated
void mainWidget::updateContributionsModel(int row)
{
//    contributionsModel->clear();
//    initializeContModel();

//    for (int i = 0; i < currentStudents.size(); i++)
//    {
//        QList<QStandardItem*> fullName;
//        QStandardItem* first = new QStandardItem(currentStudents[i].getFirstName());
//        first->setFlags(first->flags() & ~Qt::ItemIsEditable);  //changing the item flags to make the student name non editable from the contributions page
//        QStandardItem* last = new QStandardItem(currentStudents[i].getLastName());
//        last->setFlags(last->flags() & ~Qt::ItemIsEditable);
//        fullName.append(first);
//        fullName.append(last);

//        for (int j = 0; j < eventNames.size(); j++)
//        {
//            contributionsModel->setHorizontalHeaderItem(j+2, new QStandardItem(eventNames[j])); //adding all the event names to the header
//            fullName.append(new QStandardItem(currentStudents[i].getStudentEvent(j)));
//        }

//        contributionsModel->appendRow(fullName);
//    }
//    writeToContributionsFile();
//    writeToServiceFile();
//    ui->contributionsTableView->setUpdatesEnabled(true);

    contributionsModel->removeRows(row,1);

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
                QVector<QString> temp;
                if (currentStudents[j].getStudentEvent(k).size() == 0)
                {
                   temp = currentStudents[j].getEventVector();
                   temp[k] = " ";
                   currentStudents[j].setEventVector(temp);
                }
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

/*-------------------END CONTRIBUTIONS TAB ON OFFICER RECORDS-------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*------------------SERVICE PROJECTS TAB ON OFFICER RECORDS------------------*/

void mainWidget::on_serveMenuButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void mainWidget::on_serveAddEventButton_clicked()
{
    disableButtons();
    //the addDialog prompts the user to enter a name for a custom event
    addDialog->show();  //see on_eventAdded under contributions section for code
}


void mainWidget::on_serveDeleteEventButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete Meeting",
                 "Are you sure you want to delete this event?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (ui->serviceTableView->currentIndex().column() != 0 && ui->serviceTableView->currentIndex().column() != 1 )
        {
            serviceModel->removeColumns(ui->serviceTableView->currentIndex().column(),1);
            contributionsModel->removeColumns(ui->serviceTableView->currentIndex().column()+1,1);
            eventNames.erase(eventNames.begin()+ui->serviceTableView->currentIndex().column() - 1);   //removes the name of the deleted event from the names vector
            //copies the current currentStudents event vector into a temporary one, deletes the specified event, and replaces the currentStudents vector with the updated one
            for (int i = 0; i < currentStudents.size(); i++)
            {
                QVector<QString> tempEvents = currentStudents[i].getServeVector();
                tempEvents.erase(tempEvents.begin()+ui->serviceTableView->currentIndex().column() - 1);
                currentStudents[i].setServeVector(tempEvents);
            }
            for (int i = 0; i < currentStudents.size(); i++)
            {
                QVector<QString> tempEvents = currentStudents[i].getEventVector();
                tempEvents.erase(tempEvents.begin()+ui->serviceTableView->currentIndex().column() - 1);
                currentStudents[i].setEventVector(tempEvents);
            }
            writeToContributionsFile();
            writeToServiceFile();
        }
    }
    else
    {
        enableButtons();
    }
}

void mainWidget::on_serveEventEdited(QString event, int row, int column)
{
    int serveCounter = 0;
    qDebug() << "Student: " << currentStudents[row].getFirstName() << currentStudents[row].getLastName();
    QVector<QString> events;
    //the current student vector is copied to the events vector so it can be updated with the edit
    for (int i = 0; i < (serviceModel->columnCount() - 2); i++)
    {
        events = currentStudents[row].getServeVector();
    }
    events[column - 2] = event;             //the edited event is added to the events vector at the index of the column
    currentStudents[row].setServeVector(events);        //copy method is called that updates the student vector with the events vector
    for (int i = 0; i < (serviceModel->columnCount() - 2); i++)
    {
        events = currentStudents[row].getServeVector();
    }
    //loops through every value in the vector to detect meetings where the student was present
    for (int k = 0; k < events.size(); k++)
    {
        if (events[k].size() > 1)
        {
            serveCounter++;
        }
    }
    //updates the students meetings counter
    currentStudents[row].setServProjects(serveCounter);
    //converts this counter to a string
    QString serveString = QString::number(serveCounter);
    QStandardItem* newValue = new QStandardItem(serveString);
    newValue->setFlags(newValue->flags() & ~Qt::ItemIsEditable);    //makes the item non editable
    currentStudentsModel->setItem(row, 3, newValue);      //updates the edited column in the current students view
    writeToFile();
    writeToServiceFile();
}

//sets default header values and parameters for the service model
void mainWidget::initializeServiceModel()
{
    ui->serviceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //setting the header text of the contributions model
    serviceModel->setHorizontalHeaderItem(0, new QStandardItem(QString("First Name")));
    serviceModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Last Name")));
}

//updates the service model when any changes are made to the student names
void mainWidget::updateServiceModel(int row)
{
//    serviceModel->clear();
//    initializeServiceModel();

//    for (int i = 0; i < currentStudents.size(); i++)
//    {
//        QList<QStandardItem*> fullName;
//        QStandardItem* first = new QStandardItem(currentStudents[i].getFirstName());
//        first->setFlags(first->flags() & ~Qt::ItemIsEditable);  //changing the item flags to make the student name non editable from the contributions page
//        QStandardItem* last = new QStandardItem(currentStudents[i].getLastName());
//        last->setFlags(last->flags() & ~Qt::ItemIsEditable);
//        fullName.append(first);
//        fullName.append(last);
//        for (int j = 0; j < eventNames.size(); j++)
//        {
//            serviceModel->setHorizontalHeaderItem(j+2, new QStandardItem(eventNames[j])); //adding all the event names to the header
//            fullName.append(new QStandardItem(currentStudents[i].getServeEvent(j)));
//        }
//        serviceModel->appendRow(fullName);
//    }
//    writeToServiceFile();
//    ui->serviceTableView->setUpdatesEnabled(true);
    serviceModel->removeRows(row,1);
    writeToServiceFile();
}

//truncates previous file data and rewrites all service event data to serviceprojects.csv
void mainWidget::writeToServiceFile()
{
    QString filename = "serviceprojects.csv";
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
                QVector<QString> temp;
                if (currentStudents[j].getServeEvent(k).size() == 0)
                {
                   temp = currentStudents[j].getServeVector();
                   temp[k] = " ";
                   currentStudents[j].setServeVector(temp);
                }
                stream << currentStudents[j].getServeEvent(k) << ",";
            }
            stream << endl;
        }
    }
}

//reads data from serviceprojects.csv into the table and the current students vector
void mainWidget::populateServiceModel()
{
    serviceModel->clear();
    initializeServiceModel();
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
        serviceModel->appendRow(fullName);
    }

    QString filename = "serviceprojects.csv";
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

                    serviceModel->setHorizontalHeaderItem(i+2, new QStandardItem(value));

                    qDebug() << "EVENT NAMES: " << eventNames[i];

                }

            lineindex++;
            }
            else if(lineindex > 0 && lineindex <= currentStudents.size())          //gets every other line afterwards, which is the student event data
            {
                for (int j = 0; j < lineToken.size(); j++)
                {
                QString value = lineToken.at(j);
                serviceModel->setItem(lineindex - 1, j + 2, new QStandardItem(value));
                currentStudents[lineindex-1].setServeEvent(value);
                }
                lineindex++;
            }
        }
    }
}

/*~~~~~~~~~~~~~~~~~~~SERVICE PROJECTS TAB ENDS~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*------------------MEETINGS TAB ON OFFICER RECORDS------------------*/


void mainWidget::on_meetingMenuButton_clicked() { ui->stackedWidget->setCurrentIndex(0); }

void mainWidget::on_addMeetingButton_clicked()
{
    disableButtons();
    meetingsDialog->show();
}

void mainWidget::on_deleteMeetingButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete Meeting",
                 "Are you sure you want to delete this meeting?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (ui->meetingsTableView->currentIndex().column() != 0 && ui->meetingsTableView->currentIndex().column() != 1 )
        {
            meetingsModel->removeColumns(ui->meetingsTableView->currentIndex().column(),1);
            dates.erase(dates.begin()+ui->meetingsTableView->currentIndex().column() - 1);   //removes the date of the deleted meeting from the dates vector
            //copies the current currentStudents meeting vector into a temporary one, deletes the specified meeting, and replaces the currentStudents vector with the updated one
            for (int i = 0; i < currentStudents.size(); i++)
            {
                QVector<bool> tempMeetings = currentStudents[i].getMeetingsBool();
                tempMeetings.erase(tempMeetings.begin()+ui->meetingsTableView->currentIndex().column() - 1);
                currentStudents[i].setMeetingsBool(tempMeetings);
            }

            writeToMeetingsFile();
        }
    }
    else
    {
        enableButtons();
    }
}

//adds the specified date to the header of the view
void mainWidget::on_dateAdded(QString date)
{

    QList<QStandardItem*> newRecord;
    dates.push_back(date);
    meetingsDialog->close();
    enableButtons();
    for (int i = 0; i < currentStudents.size(); i++)
    {
        bool newMeeting = 0;
        currentStudents[i].setMeeting(newMeeting);
        newRecord.append(new QStandardItem(""));
    }
    meetingsModel->appendColumn(newRecord);
    meetingsModel->setHorizontalHeaderItem(dates.size() + 1, new QStandardItem(date));
}

void mainWidget::on_cancelMeetingsButtonClicked()
{
    meetingsDialog->close();
    enableButtons();
}

void mainWidget::on_meetingComboEdited(bool meetingStatus, int row, int column)
{
    qDebug() << "u in bru";
    QVector<bool> meetings;
    int meetingsCounter = 0;
    //assigns a temporary vector to the current students vector
    for (int i = 0; i < (meetingsModel->columnCount() - 2); i++)
    {
        meetings = currentStudents[row].getMeetingsBool();
    }
    //inserts the edited value into the temporary vector
    meetings[column - 2] = meetingStatus;
    //assigns the temporary vector to the current students meetings vector
    currentStudents[row].setMeetingsBool(meetings);
    //reassigns the updated current students vector to the meetings vector
    for (int i = 0; i < (meetingsModel->columnCount() - 2); i++)
    {
        meetings = currentStudents[row].getMeetingsBool();
    }
    //loops through every value in the vector to detect meetings where the student was present
    for (int k = 0; k < meetings.size(); k++)
    {
        if (meetings[k] == 1)
        {
            meetingsCounter++;
        }
    }
    //updates the students meetings counter
    currentStudents[row].setAttendedMeetings(meetingsCounter);
    //converts this counter to a string
    QString meetingString = QString::number(meetingsCounter);
    QStandardItem* newValue = new QStandardItem(meetingString);
    newValue->setFlags(newValue->flags() & ~Qt::ItemIsEditable);    //makes the item non editable
    currentStudentsModel->setItem(row, 4, newValue);      //updates the edited column in the current students view
    writeToFile();
    writeToMeetingsFile();
}

void mainWidget::initializeMeetingsModel()
{
    ui->meetingsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //setting the header text of the contributions model
    meetingsModel->setHorizontalHeaderItem(0, new QStandardItem(QString("First Name")));
    meetingsModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Last Name")));
}

//updating the meetings model every time the current students name is updated
void mainWidget::updateMeetingsModel(int row)
{
//    meetingsModel->clear();
//    initializeMeetingsModel();

//    for (int i = 0; i < currentStudents.size(); i++)
//    {
//        QList<QStandardItem*> fullName;
//        QStandardItem* first = new QStandardItem(currentStudents[i].getFirstName());
//        first->setFlags(first->flags() & ~Qt::ItemIsEditable);  //changing the item flags to make the student name non editable from the contributions page
//        QStandardItem* last = new QStandardItem(currentStudents[i].getLastName());
//        last->setFlags(last->flags() & ~Qt::ItemIsEditable);
//        fullName.append(first);
//        fullName.append(last);
//        for (int j = 0; j < dates.size(); j++)
//        {
//            QString meeting;
//            meetingsModel->setHorizontalHeaderItem(j+2, new QStandardItem(dates[j])); //adding all the event names to the header
//            if (currentStudents[i].getMeeting(j) == 0) { meeting = "Absent"; }
//            else if (currentStudents[i].getMeeting(j) == 1) { meeting = "Present"; }
//            fullName.append(new QStandardItem(meeting));
//        }
//        meetingsModel->appendRow(fullName);
//    }
//    writeToMeetingsFile();
//    ui->meetingsTableView->setUpdatesEnabled(true);
    meetingsModel->removeRows(row,1);
    writeToMeetingsFile();
}

//writing all meeting data to a file
void mainWidget::writeToMeetingsFile()
{
    QString filename = "meetings.csv";
    QFile file(filename);
    if(file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream(&file);

        for (int i = 0; i < dates.size(); i++)
        {
            stream << dates[i] << ",";
        }
        stream << endl;
        for (int j = 0; j < currentStudents.size(); j++)
        {
            for (int k = 0; k < dates.size(); k++)
            {
                stream << currentStudents[j].getMeeting(k) << ",";
            }
            stream << endl;
        }
    }
}

//initially populating the meetings table with all stored data
void mainWidget::populateMeetingsModel()
{
    meetingsModel->clear();
    initializeMeetingsModel();
    for (int i = 0; i < currentStudents.size(); i++)
    {
        QList<QStandardItem*> fullName;
        QStandardItem* first = new QStandardItem(currentStudents[i].getFirstName());
        first->setFlags(first->flags() & ~Qt::ItemIsEditable);  //changing the item flags to make the student name non editable from the contributions page
        QStandardItem* last = new QStandardItem(currentStudents[i].getLastName());
        last->setFlags(last->flags() & ~Qt::ItemIsEditable);
        fullName.append(first);
        fullName.append(last);
        meetingsModel->appendRow(fullName);
    }
    QString filename = "meetings.csv";
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

                    meetingsModel->setHorizontalHeaderItem(i+2, new QStandardItem(value));
                    dates.push_back(value);
                    qDebug() << "DATES: " << dates[i];

                }

            lineindex++;
            }
            else if(lineindex > 0 && lineindex <= currentStudents.size())          //gets every other line afterwards, which is the student event data
            {
                QString meeting;
                for (int j = 0; j < lineToken.size(); j++)
                {
                bool value = lineToken.at(j).toUInt();
                if (value == 0) { meeting = "Absent"; }
                else if (value == 1) { meeting = "Present"; }
                meetingsModel->setItem(lineindex - 1, j + 2, new QStandardItem(meeting));
                currentStudents[lineindex-1].setMeeting(value);
                }
                lineindex++;
            }
        }
    }
}


/*------------------END MEETINGS TAB ON OFFICER RECORDS------------------*/


/*~~~~~~~~~~~~~~~~~~~ADMIN RECORDS BEGIN~~~~~~~~~~~~~~~~~*/

void mainWidget::on_offMenuButton2_clicked() { ui->stackedWidget->setCurrentIndex(0); mainWidget::setFixedSize(850, 550); }

void mainWidget::on_offAddStudentButton2_clicked()
{
    //inserts a blank record with 9 columns to be edited
   QList<QStandardItem *> newRecord;
   for (int i = 0; i < currentAdminCols; i++)
   {
     newRecord.append(new QStandardItem(" "));
   }
   currentAdminModel->appendRow(newRecord);

   ProspectStudent student;
   student.setStudentNotes(" ");
   currentProspectStudents.push_back(student);

   totalProspectStudents++;
   for (int i = 0; i < currentProspectStudents.size(); i++)
   {
       qDebug() << "STUDENT " << i << "NAME: " << currentProspectStudents[i].getFirstName();
   }
}

void mainWidget::on_offDeleteStudentButton2_clicked()
{
    disableButtons2();

    //Message box confirms whether or not the record should be deleted
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete Student",
                 "Are you sure you want to delete this student?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        adminDeleteRecord();
    }
    else {
        enableButtons2();
    }
}

void mainWidget::enableButtons2()
{
    ui->offDeleteStudentButton2->setEnabled(true);
    ui->offMenuButton2->setEnabled(true);
    ui->offAddStudentButton2->setEnabled(true);
}

void mainWidget::disableButtons2()
{
    ui->offDeleteStudentButton2->setEnabled(false);
    ui->offMenuButton2->setEnabled(false);
    ui->offAddStudentButton2->setEnabled(false);
}

//deletes the selected record from the view as well as from the vector
void mainWidget::adminDeleteRecord()
{
    //removes current student from vector
    if (totalProspectStudents != 0)
    {
        currentProspectStudents.erase(currentProspectStudents.begin()+ui->currentTableView_2->currentIndex().row());
        totalProspectStudents--;
    }
    currentAdminModel->removeRows(ui->currentTableView_2->currentIndex().row(),1);
    enableButtons2();
    writeToAdminFile();
}

//deletes the student from the prospect vector and adds them to the currentStudents vector
void mainWidget::on_promoteStudentButton_clicked()
{
    disableButtons2();

    //Message box confirms whether or not the record should be deleted
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Promote Student",
                 "Are you sure you want to promote this prospect student to a current student? (This will erase this student from the admin records)", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        //inserts a blank record with 6 columns to be edited
        QList<QStandardItem *> newRecord;
        newRecord.append(new QStandardItem(currentProspectStudents[ui->currentTableView_2->currentIndex().row()].getFirstName()));
        newRecord.append(new QStandardItem(currentProspectStudents[ui->currentTableView_2->currentIndex().row()].getLastName()));
        for (int i = 2; i < currentStudentCols-1; i++)
        {
            if (i == 2 || i == 3 || i == 4)
            {
                QStandardItem* data = new QStandardItem(" ");
                data->setFlags(data->flags() & ~Qt::ItemIsEditable);
                newRecord.append(data);
            }
            else
            {
                newRecord.append(new QStandardItem(" "));
            }
        }

        //appending the student's grade level
        newRecord.append(new QStandardItem(QString::number(currentProspectStudents[ui->currentTableView_2->currentIndex().row()].getStudentClass())));
        currentStudentsModel->appendRow(newRecord);

        //creating a currentstudent object and pushing it into a vector everytime a row is added
        CurrentStudent student;
        student.setFirstName(currentProspectStudents[ui->currentTableView_2->currentIndex().row()].getFirstName());
        student.setLastName(currentProspectStudents[ui->currentTableView_2->currentIndex().row()].getLastName());
        for (int j = 0; j < contributionsModel->columnCount()-2; j++)
        {
            student.setStudentEvent(" ");
            student.setServeEvent(" ");
        }
        for (int k = 0; k < meetingsModel->columnCount() - 2; k++)
        {
            student.setMeeting(false);
        }
        student.setGradeLevel(currentProspectStudents[ui->currentTableView_2->currentIndex().row()].getStudentClass());
        currentStudents.push_back(student);

        totalStudents++;

        for (int i = 0; i < currentStudents.size(); i++)
        {
            qDebug() << "STUDENT " << i << "NAME: " << currentStudents[i].getFirstName();
        }

        //update all the other officer models and write to all files
        updateModels(currentStudentsModel->rowCount()-1);
        writeToFile();
        writeToContributionsFile();
        writeToServiceFile();
        writeToMeetingsFile();

        if (totalProspectStudents != 0)
        {
            currentProspectStudents.erase(currentProspectStudents.begin()+ui->currentTableView_2->currentIndex().row());
            totalProspectStudents--;
        }
        currentAdminModel->removeRows(ui->currentTableView_2->currentIndex().row(),1);
        writeToAdminFile();
        enableButtons2();
    }
    else
    {
        enableButtons2();
    }

}
//assings data from line edits to an object in the vector based on the row number
void mainWidget::on_studentNameEdited2(ProspectStudent student, int row)
{
    qDebug() << "IN";
    currentProspectStudents[row].setFirstName(student.getFirstName());
    currentProspectStudents[row].setLastName(student.getLastName());

    qDebug() << "Student Data: " << currentProspectStudents[row].getFirstName() << ", " << currentProspectStudents[row].getLastName();
    writeToAdminFile();

}

//assings data from the combo box to an object in the vector
void mainWidget::on_studentComboEdited2(ProspectStudent student, int row)
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
   checkStudentPromo(student, row);

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
    checkStudentPromo(student, row);
}

void mainWidget::on_studentNotesEdited(ProspectStudent student, int row)
{
    currentProspectStudents[row].setStudentNotes(student.getStudentNotes());

    qDebug() << "Student Data: " << currentProspectStudents[row].getFirstName() << ", " << currentProspectStudents[row].getLastName() << ", "
             << currentProspectStudents[row].getStudentNotes();
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
        if (currentProspectStudents[i].getStudentClass() < 11 || currentProspectStudents[i].getStudentClass() > 12)
        {
            currentProspectStudents[i].setStudentClass(11);
        }
        if (currentProspectStudents[i].getStudentStatus() > 1)
        {
            currentProspectStudents[i].setStudentStatus(false);
        }
        if (currentProspectStudents[i].getStudentNotes().size() == 0)
        {
            currentProspectStudents[i].setStudentNotes(" ");
        }

        stream << currentProspectStudents[i].getFirstName() << "," << currentProspectStudents[i].getLastName() << ","
                                                   << currentProspectStudents[i].getApplicationBool() << "," << currentProspectStudents[i].getEssayBool() << ","
                                                   << currentProspectStudents[i].getRecommendationBool() << "," << currentProspectStudents[i].getApprovalBool() << ","
                                                   << currentProspectStudents[i].getStudentGpa() << "," << currentProspectStudents[i].getStudentClass() << ","
                                                   << currentProspectStudents[i].getStudentStatus() << "," << currentProspectStudents[i].getStudentNotes() << "," << endl;
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
                            newRecord.push_back(" ");
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
                            newRecord.push_back(" ");
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
                else if (student.getStudentStatus() == 1)
                {
                    QStandardItem *item = new QStandardItem("Member");
                    currentAdminModel->setItem(lineindex, 8, item);
                }


                qDebug() << "POPULATE PROSPECT STUDENT: " << student.getFirstName() << ", " << student.getLastName() << ", " << student.getApplicationBool() << ", "
                         << student.getEssayBool() << ", " << student.getRecommendationBool() << ", " << student.getApprovalBool() << ", "
                         << student.getStudentGpa() << ", " << student.getStudentClass() << ", " << student.getStudentStatus() << ", " << student.getStudentNotes();

                totalProspectStudents++;
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

void mainWidget::checkStudentPromo(ProspectStudent student, int row)
{
    if (currentProspectStudents[row].getApplicationBool() == 1 && currentProspectStudents[row].getEssayBool() == 1 && currentProspectStudents[row].getRecommendationBool() == 1 &&
            currentProspectStudents[row].getApprovalBool() == 1 && currentProspectStudents[row].getStudentGpa() == 1 && currentProspectStudents[row].getStudentStatus() != 1) {
        disableButtons2();

        //Message box confirms whether or not the record should be deleted
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Student Promotion",
                     "This Student has met all of the requirements: " + currentProspectStudents[row].getFirstName() + " " + currentProspectStudents[row].getLastName() + "\n\n"
                     "Would you like to promote " + currentProspectStudents[row].getFirstName() + " " + currentProspectStudents[row].getLastName() +
                                                                  " to a member and add this student to officer records?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            on_promoteStudentButton_clicked();
        }
        else {
            enableButtons2();
        }
    }
}

