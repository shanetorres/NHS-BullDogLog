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
    ProspectStudent(QString, QString, bool, bool, bool, bool, QString);

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

    void setStudentGpa(QString);
    QString getStudentGpa();

private:
    QString firstName;
    QString lastName;
    bool application;
    bool essay;
    bool teach;
    bool board;
    QString studentGpa;
};

#endif // PROSPECTSTUDENT_H
