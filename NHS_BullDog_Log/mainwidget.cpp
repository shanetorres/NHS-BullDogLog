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
    connect(currentAdminDelegate, SIGNAL(checkStudentPromo(int)), this, SLOT(checkStudentPromo(int)));
    connect(currentAdminDelegate, SIGNAL(prospectEdited(int)), this, SLOT(on_prospectEdited(int)));


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
    connect(contributionDelegate, SIGNAL(eventEdited(int,int)), this, SLOT(on_eventEdited(int,int)));



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

    connect(serviceDelegate, SIGNAL(serveEventEdited(int,int)), this, SLOT(on_serveEventEdited(int,int)));

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
    connect(meetingsDelegate, SIGNAL(meetingComboEdited(int,int)), this, SLOT(on_meetingComboEdited(int,int)));

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
    if (ui->stackedWidget->currentIndex() == 1 && (index == 0 || index == 1)) //admin records sorting
    {
        if (sortOrder[0] == 0)
        {
            currentAdminModel->sort(index,Qt::AscendingOrder);
            sortOrder[0] = 1;
        }
        else if (sortOrder[0] == 1)
        {
            currentAdminModel->sort(index,Qt::DescendingOrder);
            sortOrder[0] = 0;
        }
        writeToAdminFile();
    }
    if (ui->stackedWidget->currentIndex() == 2)     //officer records sorting
    {
        if (ui->tabWidget->currentIndex() == 0)     //overall tab sorting
        {
            //checks to see i

            if (sortOrder[1] == 0 && (index == 0 || index == 1)) //if the sort order is ascending and the index is the first or last name header
            {
                //all models are sorted to prevent from information getting mixed up between the models
                currentStudentsModel->sort(index,Qt::AscendingOrder);
                contributionsModel->sort(index,Qt::AscendingOrder);
                serviceModel->sort(index,Qt::AscendingOrder);
                meetingsModel->sort(index,Qt::AscendingOrder);
                sortOrder[1] = 1;       //toggles between 0 and 1 to allow for alternating ascending and descending sorts
                //sort orders are save to files
                writeToFile();
                writeToContributionsFile();
                writeToServiceFile();
                writeToMeetingsFile();
            }
            else if (sortOrder[1] == 1  && (index == 0 || index == 1))
            {
                currentStudentsModel->sort(index,Qt::DescendingOrder);
                contributionsModel->sort(index,Qt::DescendingOrder);
                serviceModel->sort(index,Qt::DescendingOrder);
                meetingsModel->sort(index,Qt::DescendingOrder);
                sortOrder[1] = 0;
                writeToFile();
                writeToContributionsFile();
                writeToServiceFile();
                writeToMeetingsFile();
            }
        }
        if (ui->tabWidget->currentIndex() == 1) //contributions model sorting
        {
            if (sortOrder[2] == 0 && (index == 0 || index == 1))
            {
                //all models are sorted to prevent from information getting mixed up between the models
                currentStudentsModel->sort(index,Qt::AscendingOrder);
                contributionsModel->sort(index,Qt::AscendingOrder);
                serviceModel->sort(index,Qt::AscendingOrder);
                meetingsModel->sort(index,Qt::AscendingOrder);
                sortOrder[2] = 1;       //toggles between 0 and 1 to allow for alternating ascending and descending sorts
                //sort orders are save to files
                writeToFile();
                writeToContributionsFile();
                writeToServiceFile();
                writeToMeetingsFile();
            }
            if (sortOrder[2] == 1 && (index == 0 || index == 1))
            {
                currentStudentsModel->sort(index,Qt::DescendingOrder);
                contributionsModel->sort(index,Qt::DescendingOrder);
                serviceModel->sort(index,Qt::DescendingOrder);
                meetingsModel->sort(index,Qt::DescendingOrder);
                sortOrder[2] = 0;
                writeToFile();
                writeToContributionsFile();
                writeToServiceFile();
                writeToMeetingsFile();
            }
        }
    }
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
                textData += ",";     // for .csv file format
        }
        textData += "\n";             // (optional: for new line segmentation)
    }

    // [Save to file] (header file <QFile> needed)
    // .csv

    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
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
    for (int i = 0; i < currentStudentsModel->rowCount(); i++)
    {
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
            eventNames.erase(eventNames.begin()+ui->contributionsTableView->currentIndex().column() - 1);   //removes the name of the deleted event from the names vector
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
void mainWidget::on_eventEdited(int row, int column)
{
    int contributionsCounter = 0;

    //loops through every value in the model to detect contributioins
    for (int k = 2; k < contributionsModel->columnCount(); k++)
    {
        if (contributionsModel->data(contributionsModel->index(row,k)).toString().size() > 1)
        {
            contributionsCounter++;
        }
    }
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
    contributionsModel->removeRows(row,1);
    writeToContributionsFile();
}

//writes the event names as well as each student's contribution to a file
void mainWidget::writeToContributionsFile()
{
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
        // [Collect model data to QString]
        QString textData;
        int rows = contributionsModel->rowCount();
        int columns = contributionsModel->columnCount();

        for (int i = 0; i < rows; i++) {
            for (int j = 2; j < columns; j++) {

                    textData += contributionsModel->data(contributionsModel->index(i,j)).toString();
                    textData += ",";     // for .csv file format
            }
            textData += "\n";             // (optional: for new line segmentation)
        }

        stream << textData;

        file.close();
    }

}


//populates the contributions tab with current students names and event data from contributions.csv
void mainWidget::populateContributionsModel()
{
    contributionsModel->clear();
    initializeContModel();
    //sets the data for the first two columns to each student's first and last name
    for (int i = 0; i < currentStudentsModel->rowCount(); i++)
    {
        QList<QStandardItem*> fullName;
        QStandardItem* first = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(i,0)).toString());
        first->setFlags(first->flags() & ~Qt::ItemIsEditable);      //changing the item flags to make the student name non editable from the contributions page
        QStandardItem* last = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(i,1)).toString());
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
            else if(lineindex > 0 && lineindex <= currentStudentsModel->rowCount())          //gets every other line afterwards, which is the student event data
            {
                for (int j = 0; j < lineToken.size(); j++)
                {
                QString value = lineToken.at(j);
                contributionsModel->setItem(lineindex - 1, j + 2, new QStandardItem(value));
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
            writeToContributionsFile();
            writeToServiceFile();
        }
    }
    else
    {
        enableButtons();
    }
}

void mainWidget::on_serveEventEdited(int row, int column)
{
    int serviceCounter = 0;

    //loops through every value in the model to detect contributioins
    for (int k = 2; k < serviceModel->columnCount(); k++)
    {
        if (serviceModel->data(serviceModel->index(row,k)).toString().size() > 1)
        {
            serviceCounter++;
        }
    }

    //converts this counter to a string
    QString serveString = QString::number(serviceCounter);
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
        // [Collect model data to QString]
        QString textData;
        int rows = serviceModel->rowCount();
        int columns = serviceModel->columnCount();

        for (int i = 0; i < rows; i++) {
            for (int j = 2; j < columns; j++) {

                    textData += serviceModel->data(serviceModel->index(i,j)).toString();
                    textData += ",";     // for .csv file format
            }
            textData += "\n";             // (optional: for new line segmentation)
        }

        stream << textData;

        file.close();
    }
}

//reads data from serviceprojects.csv into the table and the current students vector
void mainWidget::populateServiceModel()
{
    serviceModel->clear();
    initializeServiceModel();
    //sets the data for the first two columns to each student's first and last name
    for (int i = 0; i < currentStudentsModel->rowCount(); i++)
    {
        QList<QStandardItem*> fullName;
        QStandardItem* first = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(i,0)).toString());
        first->setFlags(first->flags() & ~Qt::ItemIsEditable);      //changing the item flags to make the student name non editable from the contributions page
        QStandardItem* last = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(i,1)).toString());
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
            else if(lineindex > 0 && lineindex <= currentStudentsModel->rowCount())          //gets every other line afterwards, which is the student event data
            {
                for (int j = 0; j < lineToken.size(); j++)
                {
                QString value = lineToken.at(j);
                serviceModel->setItem(lineindex - 1, j + 2, new QStandardItem(value));
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
            dates.erase(dates.begin()+ui->meetingsTableView->currentIndex().column() - 1);   //removes the date of the deleted meeting from the dates vecto
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
    for (int i = 0; i < currentStudentsModel->rowCount(); i++)
    {
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

void mainWidget::on_meetingComboEdited(int row, int column)
{
    int meetingsCounter = 0;
    //loops through every value in the model to detect meetings where the student was present
    for (int k = 2; k < meetingsModel->columnCount(); k++)
    {
        if (meetingsModel->data(meetingsModel->index(row,k)).toString() == "Present")
        {
            meetingsCounter++;
        }
    }
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
        // [Collect model data to QString]
        QString textData;
        int rows = meetingsModel->rowCount();
        int columns = meetingsModel->columnCount();

        for (int i = 0; i < rows; i++) {
            for (int j = 2; j < columns; j++) {

                    textData += meetingsModel->data(meetingsModel->index(i,j)).toString();
                    textData += ",";     // for .csv file format
            }
            textData += "\n";             // (optional: for new line segmentation)
        }

        stream << textData;

        file.close();
    }

}

//initially populating the meetings table with all stored data
void mainWidget::populateMeetingsModel()
{
    meetingsModel->clear();
    initializeMeetingsModel();
    for (int i = 0; i < currentStudentsModel->rowCount(); i++)
    {
        QList<QStandardItem*> fullName;
        QStandardItem* first = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(i,0)).toString());
        first->setFlags(first->flags() & ~Qt::ItemIsEditable);      //changing the item flags to make the student name non editable from the contributions page
        QStandardItem* last = new QStandardItem(currentStudentsModel->data(currentStudentsModel->index(i,1)).toString());
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
            else if(lineindex > 0 && lineindex <= currentStudentsModel->rowCount())          //gets every other line afterwards, which is the student event data
            {
                for (int j = 0; j < lineToken.size(); j++)
                {

                meetingsModel->setItem(lineindex - 1, j + 2, new QStandardItem(lineToken[j]));
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
  QList<QStandardItem *> newRecord;
  for (int i = 0; i < currentAdminModel->columnCount(); i++)
  {
      newRecord.append(new QStandardItem(" "));
  }
  currentAdminModel->appendRow(newRecord);
    writeToAdminFile();
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
        newRecord.append(new QStandardItem(currentAdminModel->data(currentAdminModel->index(ui->currentTableView_2->currentIndex().row(),0)).toString()));
        newRecord.append(new QStandardItem(currentAdminModel->data(currentAdminModel->index(ui->currentTableView_2->currentIndex().row(),1)).toString()));
        for (int i = 2; i < currentStudentCols-1; i++)
        {
             newRecord.append(new QStandardItem(" "));
        }

        //appending the student's grade level
        newRecord.append(new QStandardItem(currentAdminModel->data(currentAdminModel->index(ui->currentTableView_2->currentIndex().row(),7)).toString()));
        currentStudentsModel->appendRow(newRecord);


        //update all the other officer models and write to all files
        updateModels(currentStudentsModel->rowCount()-1);
        writeToFile();
        writeToContributionsFile();
        writeToServiceFile();
        writeToMeetingsFile();
        currentAdminModel->removeRows(ui->currentTableView_2->currentIndex().row(),1);
        writeToAdminFile();
        enableButtons2();
    }
    else
    {
        enableButtons2();
    }

}

void mainWidget::on_prospectEdited(int row)
{
    writeToAdminFile();
}

//writes all objects in the current prospect student vector to a file
void mainWidget::writeToAdminFile()
{
    QString filename = "currentprospectstudents.csv";
    QFile file(filename);

    // [Collect model data to QString]
    QString textData;
    int rows = currentAdminModel->rowCount();
    int columns = currentAdminModel->columnCount();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {

                textData += currentAdminModel->data(currentAdminModel->index(i,j)).toString();
                textData += ",";     // for .csv file format
        }
        textData += "\n";             // (optional: for new line segmentation)
    }

    // [Save to file] (header file <QFile> needed)
    // .csv

    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << textData;

        file.close();
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

                    for (int i = 0; i < lineToken.size(); i++)
                    {
                        QString value = lineToken.at(i);
                        newRecord.push_back(value);
                        QStandardItem *item = new QStandardItem(value);
                        currentAdminModel->setItem(lineindex, i, item);
                    }
            lineindex++;
        }
    }
    }
}

void mainWidget::checkStudentPromo(int row)
{
    if (currentAdminModel->data(currentAdminModel->index(row,2)).toString() == "1" && currentAdminModel->data(currentAdminModel->index(row,3)).toString() == "1" && currentAdminModel->data(currentAdminModel->index(row,4)).toString() == "1" &&
            currentAdminModel->data(currentAdminModel->index(row,5)).toString() == "1" && currentAdminModel->data(currentAdminModel->index(row,6)).toString() == "1" && currentAdminModel->data(currentAdminModel->index(row,8)).toString() != "1") {
        disableButtons2();

        //Message box confirms whether or not the record should be deleted
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Student Promotion",
                     "This Student has met all of the requirements: " + currentAdminModel->data(currentAdminModel->index(row,0)).toString() + " " + currentAdminModel->data(currentAdminModel->index(row,1)).toString() + "\n\n"
                     "Would you like to promote " + currentAdminModel->data(currentAdminModel->index(row,0)).toString() + " " + currentAdminModel->data(currentAdminModel->index(row,1)).toString() +
                                                                  " to a member and add this student to officer records?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            on_promoteStudentButton_clicked();
        }
        else {
            enableButtons2();
        }
    }
}

