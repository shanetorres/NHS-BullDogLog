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
    CurrentStudent(QString, QString, int, int, int, bool, int);

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

    void setGradeLevel(int);
    int getGradeLevel();

    void setStudentEvent(QString);
    QString getStudentEvent(int);

    void setEventVector(QVector<QString>);
    QVector<QString> getEventVector();


    void setServeEvent(QString);
    QString getServeEvent(int);

    void setServeVector(QVector<QString>);
    QVector<QString> getServeVector();

    void setMeetingsBool(QVector<bool>);
    QVector<bool> getMeetingsBool();


private:
    QString firstName;
    QString lastName;
    int contributions;
    int servProjects;
    int attendedMeetings;
    bool inductionAttend;
    int gradeLevel;
    QVector<QString> studentEvents;

    QVector<QString> serveEvents;
    QVector<bool> meetingsBool;

};

#endif // CURRENTSTUDENT_H
