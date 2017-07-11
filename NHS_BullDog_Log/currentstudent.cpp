#include "currentstudent.h"

CurrentStudent::CurrentStudent()
{

}

//default constructor assigns all inputted data to data members of the class via the set functions
CurrentStudent::CurrentStudent(QString first, QString last, QString cont, QString serv, QString meetings, QString induct)
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

void CurrentStudent::setContributions(QString cont) { contributions = cont; }

QString CurrentStudent::getContributions() { return contributions; }

void CurrentStudent::setServProjects(QString serv) { servProjects = serv; }

QString CurrentStudent::getServProjects() { return servProjects; }

void CurrentStudent::setAttendedMeetings(QString meetings) { attendedMeetings = meetings; }

QString CurrentStudent:: getAttendedMeetings() { return attendedMeetings; }

void CurrentStudent::setInductionAttendance(QString induct) { inductionAttend = induct; }

QString CurrentStudent::getInductionAttendance() { return inductionAttend; }
