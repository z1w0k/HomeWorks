#ifndef VMIK_FACULTY_H
#define VMIK_FACULTY_H

#include "Faculty.h"
#include <vector>

class VmikFaculty : virtual public Faculty {
public:
    VmikFaculty();
    virtual ~VmikFaculty() {}

    ExamResult takeExam(const StudentInfo& student, const Exam& exam) const override;
    std::string getExamFormat(const Exam& exam) const override;
    void displayInfo() const override;
};

#endif