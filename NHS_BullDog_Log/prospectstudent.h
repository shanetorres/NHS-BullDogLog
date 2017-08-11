#ifndef PROSPECTSTUDENT_H
#define PROSPECTSTUDENT_H
#include <QString>
#include "admindelegate.h"
#include <QVector>

class adminDelegate;

class ProspectStudent
{
public:
    ProspectStudent();
    ProspectStudent(QString, QString, bool, bool, bool, bool, bool, int, bool, QString);

    void setFirstName(QString);
    QString getFirstName();

    void setLastName(QString);
    QString getLastName();

    void setApplicationBool(bool);
    bool getApplicationBool();

    void setEssayBool(bool);
    bool getEssayBool();

    void setRecommendationBool(bool);
    bool getRecommendationBool();

    void setApprovalBool(bool);
    bool getApprovalBool();

    void setStudentGpa(bool);
    bool getStudentGpa();

    void setStudentClass(int);
    int getStudentClass();

    void setStudentStatus(bool);
    bool getStudentStatus();

    void setStudentNotes(QString);
    QString getStudentNotes();

private:
    QString firstName;
    QString lastName;
    bool application;
    bool essay;
    bool teach;
    bool board;
    bool studentGpa;
    int studentClass;
    bool studentStatus;
    QString studentNotes;
};

#endif // PROSPECTSTUDENT_H
