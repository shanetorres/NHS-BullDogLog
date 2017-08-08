#ifndef CURRENTSTUDENT_H
#define CURRENTSTUDENT_H
#include <QString>
#include "officerdelegate.h"
#include <QVector>

class OfficerDelegate;

class CurrentStudent
{
public:
    CurrentStudent();
    CurrentStudent(QString, QString, int, int, int, bool);

    void setFirstName(QString);
    QString getFirstName();

    void setLastName(QString);
    QString getLastName();

    void setContributions(int);
    int getContributions();

    void setServProjects(int);
    int getServProjects();

    void setAttendedMeetings(int);
    int getAttendedMeetings();

    void setInductionAttendance(bool);
    bool getInductionAttendance();

    void setStudentEvent(QString);
    QString getStudentEvent(int);

    void setEventVector(QVector<QString>);
    QVector<QString> getEventVector();

    //admin set and get methods

    void setApplicationBool(bool);
    bool getApplicationBool();

    void setEssayBool(bool);
    bool getEssayBool();

    void setRecommendationBool(bool);
    bool getRecommendationBool();

    void setApprovalBool(bool);
    bool getApprovalBool();

    void setStudentGpa(float);
    float getStudentGpa();

private:
    QString firstName;
    QString lastName;
    int contributions;
    int servProjects;
    int attendedMeetings;
    bool inductionAttend;
    QVector<QString> studentEvents;
    //admin records
    bool application;
    bool essay;
    bool teach;
    bool board;
    float studentGpa;
};

#endif // CURRENTSTUDENT_H
