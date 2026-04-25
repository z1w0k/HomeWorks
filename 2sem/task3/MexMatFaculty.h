#ifndef MEX_MAT_FACULTY_H
#define MEX_MAT_FACULTY_H

#include "Faculty.h"

class MexMatFaculty : virtual public Faculty {
private:
    bool requiresCalculators;

public:
    MexMatFaculty();
    virtual ~MexMatFaculty() {}

    ExamResult takeExam(const StudentInfo& student, const Exam& exam) const override;
    std::string getExamFormat(const Exam& exam) const override;
    void displayInfo() const override;

    bool getRequiresCalculators() const { return requiresCalculators; }
};

#endif