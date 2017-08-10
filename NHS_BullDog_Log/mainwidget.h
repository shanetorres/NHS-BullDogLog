#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "officerdelegate.h"
#include "contributiondelegate.h"
#include "servicedelegate.h"
#include "currentstudent.h"
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

    //service projects page

    void initializeServiceModel();

    void populateServiceModel();

    void updateServiceModel();

    void writeToServiceFile();

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

    void on_studentGradeEdited(CurrentStudent, int);

    /*~CONTRIBUTIONS TAB~*/

    void on_contMenuButton_clicked();

    void on_contAddEventButton_clicked();

    void on_eventAdded(QString);

    void on_contDeleteEventButton_clicked();

    void on_cancelClicked();

    void on_eventEdited(QString, int, int);

    /*SERVICE PROJECTS TAB*/

    void on_serveMenuButton_clicked();

    void on_serveAddEventButton_clicked();

    void on_serveDeleteEventButton_clicked();

    void on_serveEventEdited(QString, int, int);

private:
    Ui::mainWidget *ui;
    /*~OVERALL TAB~*/
    QStandardItemModel* currentStudentsModel;
    const int currentStudentCols = 7;
    officerDelegate *currentStudentsDelegate;
    std::vector<CurrentStudent> currentStudents;
    int totalStudents;
    /*~CONTRIBUTIONS TAB~*/
    QStandardItemModel* contributionsModel;
    AddContributionDialog* addDialog;
    int contCols;
    ContributionDelegate *contributionDelegate;
    QVector<QString> eventNames;
    /*~SERVICE PROJECTS TAB~*/
    QStandardItemModel* serviceModel;
    ServiceDelegate* serviceDelegate;

};
#endif // MAINWIDGET_H
