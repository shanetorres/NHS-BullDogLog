#ifndef CURRENTSTUDENT_H
#define CURRENTSTUDENT_H
#include <QString>
#include "officerdelegate.h"

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


private:
    QString firstName;
    QString lastName;
    int contributions;
    int servProjects;
    int attendedMeetings;
    bool inductionAttend;
};

#endif // CURRENTSTUDENT_H
