#ifndef HUMANITIES_FACULTY_H
#define HUMANITIES_FACULTY_H

#include "Faculty.h"

class HumanitiesFaculty : virtual public Faculty {
public:
    HumanitiesFaculty();
    virtual ~HumanitiesFaculty() {}

    ExamResult takeExam(const StudentInfo& student, const Exam& exam) const override;
    std::string getExamFormat(const Exam& exam) const override;
    void displayInfo() const override;
};

#endif