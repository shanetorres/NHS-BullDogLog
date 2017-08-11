#include "currentstudent.h"

CurrentStudent::CurrentStudent()
{

}

//default constructor assigns all inputted data to data members of the class via the set functions
CurrentStudent::CurrentStudent(QString first, QString last, int cont, int serv, int meetings, bool induct, int grade)
{
    setFirstName(first);
    setLastName(last);
    setContributions(cont);
    setServProjects(serv);
    setAttendedMeetings(meetings);
    setInductionAttendance(induct);
    setGradeLevel(grade);
}

void CurrentStudent::setFirstName(QString first) { firstName = first; }
QString CurrentStudent::getFirstName() { return firstName; }

void CurrentStudent::setLastName(QString last) { lastName = last; }
QString CurrentStudent::getLastName() { return lastName; }

void CurrentStudent::setContributions(int cont) { contributions = cont; }
int CurrentStudent::getContributions() { return contributions; }

void CurrentStudent::setServProjects(int serv) { servProjects = serv; }
int CurrentStudent::getServProjects() { return servProjects; }

void CurrentStudent::setAttendedMeetings(int meetings) { attendedMeetings = meetings; }
int CurrentStudent:: getAttendedMeetings() { return attendedMeetings; }

void CurrentStudent::setInductionAttendance(bool induct) { inductionAttend = induct; }
bool CurrentStudent::getInductionAttendance() { return inductionAttend; }

void CurrentStudent::setGradeLevel(int grade) { gradeLevel = grade; }
int CurrentStudent::getGradeLevel() { return gradeLevel; }

void CurrentStudent::setStudentEvent(QString event) { studentEvents.push_back(event); }
QString CurrentStudent::getStudentEvent(int i) { return studentEvents[i]; }

void CurrentStudent::setEventVector(QVector<QString> events)
{
    studentEvents.clear();
    for (int i = 0; i < events.size(); i++)
    {
        studentEvents.push_back(" ");
    }
    for (int i = 0; i < events.size(); i++)
    {
        studentEvents[i] = events[i];
    }
}

QVector<QString> CurrentStudent::getEventVector()
{
    return studentEvents;
}

void CurrentStudent::setServeEvent(QString event) { serveEvents.push_back(event); }
QString CurrentStudent::getServeEvent(int i) { return serveEvents[i]; }

void CurrentStudent::setServeVector(QVector<QString> events)
{
    serveEvents.clear();
    for (int i = 0; i < events.size(); i++)
    {
        serveEvents.push_back(" ");
    }
    for (int i = 0; i < events.size(); i++)
    {
        serveEvents[i] = events[i];
    }
}

QVector<QString> CurrentStudent::getServeVector()
{
    return serveEvents;
}

void CurrentStudent::setMeetingsBool(QVector<bool> meetings)
{
    meetingsBool.clear();
    for (int i = 0; i < meetings.size(); i++)
    {
        meetingsBool.push_back(0);
    }
    for (int i = 0; i < meetings.size(); i++)
    {
        meetingsBool[i] = meetings[i];
    }
}

QVector<bool> CurrentStudent::getMeetingsBool()
{
    return meetingsBool;
}
