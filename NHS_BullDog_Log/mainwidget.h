#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "officerdelegate.h"
#include "admindelegate.h"
#include "contributiondelegate.h"
#include "currentstudent.h"
#include "prospectstudent.h"
#include "addcontributiondialog.h"
#include <vector>
#include <QFile>
#include <QTextStream>

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

    //contributions page

    void populateCurrentStudentsModel();

    void updateContributionsModel();

    void initializeContModel();

    void writeToContributionsFile();

    void populateContributionsModel();

    //admin records

    void adminDeleteRecord();

    void writeToAdminFile();

    void populateCurrentProspectStudentsModel();

private slots:

    void on_adminButton_clicked();

    void on_officerButton_clicked();

    void on_quitButton_clicked();

    /*~OVERALL TAB~*/

    void on_offMenuButton_clicked();

    void on_offAddStudentButton_clicked();

    void on_offDeleteStudentButton_clicked();

    //Signals from delegates

    void on_studentNameEdited(CurrentStudent, int);

    void on_studentSpinEdited(CurrentStudent, int);

    void on_studentComboEdited(CurrentStudent, int);

    /*~CONTRIBUTIONS TAB~*/

    void on_contMenuButton_clicked();

    void on_contAddEventButton_clicked();

    void on_eventAdded(QString);

    void on_contDeleteEventButton_clicked();

    void on_cancelClicked();

    void on_eventEdited(QString, int, int);

    //ADMIN RECORDS

    void on_offMenuButton_2_clicked();

    void on_offAddStudentButton_2_clicked();

    void on_offDeleteStudentButton_2_clicked();

    //ADMIN SIGNALS

    void on_studentNameEdited_2(ProspectStudent, int);

    void on_studentComboEdited_2(ProspectStudent, int);

    void on_studentClassEdited(ProspectStudent, int);

    void on_studentStatusEdited(ProspectStudent, int);

    //void on_studentNotesEdited(ProspectStudent,int);

private:
    Ui::mainWidget *ui;
    /*~OVERALL TAB~*/
    QStandardItemModel* currentStudentsModel;
    const int currentStudentCols = 6;
    officerDelegate *currentStudentsDelegate;
    std::vector<CurrentStudent> currentStudents;
    int totalStudents;
    /*~CONTRIBUTIONS TAB~*/
    QStandardItemModel* contributionsModel;
    AddContributionDialog* addDialog;
    int contCols;
    ContributionDelegate *contributionDelegate;
    QVector<QString> eventNames;
    /*~ADMIN RECORDS~*/
    QStandardItemModel* currentAdminModel;
    const int currentAdminCols = 10;
    adminDelegate *currentAdminDelegate;
    std::vector<ProspectStudent> currentProspectStudents;
    int totalProspectStudents; 
};
#endif // MAINWIDGET_H
