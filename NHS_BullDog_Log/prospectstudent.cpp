#include "prospectstudent.h"

ProspectStudent::ProspectStudent()
{

}

//default constructor assigns all inputted data to data members of the class via the set functions
ProspectStudent::ProspectStudent(QString first, QString last, bool app, bool ess, bool rec, bool boa, bool gpa, int cla, bool sta, QString notes)
{
    setFirstName(first);
    setLastName(last);
    setApplicationBool(app);
    setEssayBool(ess);
    setRecommendationBool(rec);
    setApprovalBool(boa);
    setStudentGpa(gpa);
    setStudentClass(cla);
    setStudentStatus(sta);
    setStudentNotes(notes);
}

void ProspectStudent::setFirstName(QString first) { firstName = first; }
QString ProspectStudent::getFirstName() { return firstName; }

void ProspectStudent::setLastName(QString last) { lastName = last; }
QString ProspectStudent::getLastName() { return lastName; }

void ProspectStudent::setApplicationBool(bool app) { application = app; }
bool ProspectStudent::getApplicationBool() { return application; }

void ProspectStudent::setEssayBool(bool ess) { essay = ess; }
bool ProspectStudent::getEssayBool() { return essay; }

void ProspectStudent::setRecommendationBool(bool rec) { teach = rec; }
bool ProspectStudent::getRecommendationBool() { return teach; }

void ProspectStudent::setApprovalBool(bool boa) { board = boa; }
bool ProspectStudent::getApprovalBool() { return board; }

void ProspectStudent::setStudentGpa(bool gpa) { studentGpa = gpa; }
bool ProspectStudent::getStudentGpa() { return studentGpa; }

void ProspectStudent::setStudentClass(int cla) { studentClass = cla; }
int ProspectStudent::getStudentClass() { return studentClass; }

void ProspectStudent::setStudentStatus(bool sta) { studentStatus = sta; }
bool ProspectStudent::getStudentStatus() { return studentStatus; }

void ProspectStudent::setStudentNotes(QString notes) { studentNotes = notes; }
QString ProspectStudent::getStudentNotes() { return studentNotes; }
