#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "officerdelegate.h"
#include "currentstudent.h"
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
    void officerDeleteRecord();
    void writeToFile();


private slots:

    void on_adminButton_clicked();

    void on_officerButton_clicked();

    void on_quitButton_clicked();

    void on_offMenuButton_clicked();

    void on_offAddStudentButton_clicked();

    void on_offDeleteStudentButton_clicked();

    //Signals from delegates

    void on_studentNameEdited(CurrentStudent, int);
    void on_studentSpinEdited(CurrentStudent, int);
    void on_studentComboEdited(CurrentStudent, int);

private:
    Ui::mainWidget *ui;
    QStandardItemModel* currentStudentsModel;
    const int currentStudentCols = 6;
    officerDelegate *currentStudentsDelegate;
    std::vector<CurrentStudent> currentStudents;
    int totalStudents;
};
#endif // MAINWIDGET_H
