#include "currentstudent.h"

CurrentStudent::CurrentStudent()
{

}

//default constructor assigns all inputted data to data members of the class via the set functions
CurrentStudent::CurrentStudent(QString first, QString last, int cont, int serv, int meetings, bool induct)
{
    setFirstName(first);
    setLastName(last);
    setContributions(cont);
    setServProjects(serv);
    setAttendedMeetings(meetings);
    setInductionAttendance(induct);
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

void CurrentStudent::setStudentEvent(QString event) { studentEvents.push_back(event); }

QString CurrentStudent::getStudentEvent(int i) { return studentEvents[i]; }

void CurrentStudent::setEventVector(QVector<QString> events)
{
    for (int i = 0; i < events.size(); i++)
    {
        studentEvents[i] = events[i];
    }
}
