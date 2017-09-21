#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "officerdelegate.h"
#include "admindelegate.h"
#include "contributiondelegate.h"
#include "servicedelegate.h"
#include "addcontributiondialog.h"
#include "addmeetingsdialog.h"
#include "meetingsdelegate.h"
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QSortFilterProxyModel>

namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = 0);

    ~mainWidget();

    void enableButtons();

    void disableButtons();

    void officerDeleteRecord();

    void writeToFile();

    void updateModels(int);

    //contributions page

    void populateCurrentStudentsModel();

    void updateContributionsModel(int);

    void initializeContModel();

    void writeToContributionsFile();

    void populateContributionsModel();

    //service projects page

    void initializeServiceModel();

    void populateServiceModel();

    void updateServiceModel(int);

    void writeToServiceFile();

    //meetings page

    void populateMeetingsModel();

    void initializeMeetingsModel();

    void updateMeetingsModel(int);

    void writeToMeetingsFile();

    //admin records

    void adminDeleteRecord();

    void writeToAdminFile();    
    
    void enableButtons2();

    void disableButtons2();

    void populateCurrentProspectStudentsModel();

private slots:

    void on_adminButton_clicked();

    void on_officerButton_clicked();

    void on_optionsButton_clicked();

    void on_quitButton_clicked();

    /*~OVERALL TAB~*/

    void on_offMenuButton_clicked();

    void on_offAddStudentButton_clicked();

    void on_offDeleteStudentButton_clicked();

    void on_sectionClicked(int);

    void on_studentEdited(int);

    /*~CONTRIBUTIONS TAB~*/

    void on_contMenuButton_clicked();

    void on_contAddEventButton_clicked();

    void on_eventAdded(QString);

    void on_contDeleteEventButton_clicked();

    void on_cancelClicked();

    void on_eventEdited(int, int);

    /*~SERVICE PROJECTS TAB~*/

    void on_serveMenuButton_clicked();

    void on_serveAddEventButton_clicked();

    void on_serveDeleteEventButton_clicked();

    void on_serveEventEdited(int, int);

    /*~MEETINGS TAB~*/

    void on_meetingMenuButton_clicked();

    void on_addMeetingButton_clicked();

    void on_deleteMeetingButton_clicked();

    void on_dateAdded(QString);

    void on_meetingComboEdited(int, int);

    void on_cancelMeetingsButtonClicked();
    
     //ADMIN RECORDS

    void on_offMenuButton2_clicked();

    void on_offAddStudentButton2_clicked();

    void on_offDeleteStudentButton2_clicked();

    //ADMIN SLOTS

    void on_prospectEdited(int);

    void checkStudentPromo(int);

    void on_promoteStudentButton_clicked();



private:
    Ui::mainWidget *ui;
    bool sortOrder[4];
    /*~OVERALL TAB~*/
    QSortFilterProxyModel* currentStudentsSortModel;
    QStandardItemModel* currentStudentsModel;
    const int currentStudentCols = 7;
    officerDelegate *currentStudentsDelegate;
    int totalStudents;
    QHeaderView *currentTableHeader;
    /*~CONTRIBUTIONS TAB~*/
    QStandardItemModel* contributionsModel;
    AddContributionDialog* addDialog;
    int contCols;
    ContributionDelegate *contributionDelegate;
    QVector<QString> eventNames;
    QHeaderView *contributionHeader;
    /*~SERVICE PROJECTS TAB~*/
    QStandardItemModel* serviceModel;
    ServiceDelegate* serviceDelegate;
    QHeaderView *serviceHeader;
    /*~MEETINGS TAB~*/
    QStandardItemModel* meetingsModel;
    AddMeetingsDialog* meetingsDialog;
    QVector<QString> dates;
    MeetingsDelegate* meetingsDelegate;
    QHeaderView *meetingsHeader;
    /*~ADMIN RECORDS~*/
    QStandardItemModel* currentAdminModel;
    const int currentAdminCols = 10;
    adminDelegate *currentAdminDelegate;
    int totalProspectStudents;
    QHeaderView *prospectHeader;
};
#endif // MAINWIDGET_H
