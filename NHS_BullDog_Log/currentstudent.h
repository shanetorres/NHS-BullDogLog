#ifndef CURRENTSTUDENT_H
#define CURRENTSTUDENT_H
#include <QString>
#include "officerdelegate.h"

class OfficerDelegate;

class CurrentStudent
{
public:
    CurrentStudent();
    CurrentStudent(QString, QString, QString, QString, QString, QString);
    void setFirstName(QString);
    QString getFirstName();
    void setLastName(QString);
    QString getLastName();
    void setContributions(QString);
    QString getContributions();
    void setServProjects(QString);
    QString getServProjects();
    void setAttendedMeetings(QString);
    QString getAttendedMeetings();
    void setInductionAttendance(QString);
    QString getInductionAttendance();


private:
    QString firstName;
    QString lastName;
    QString contributions;
    QString servProjects;
    QString attendedMeetings;
    QString inductionAttend;
};

#endif // CURRENTSTUDENT_H
