#ifndef DOUBLE_FACULTY_STUDENT_H
#define DOUBLE_FACULTY_STUDENT_H

#include "MexMatFaculty.h"
#include "HumanitiesFaculty.h"
#include "StudentInfo.h"

class DoubleFacultyStudent : public MexMatFaculty, public HumanitiesFaculty {
private:
    StudentInfo studentInfo;

public:
    DoubleFacultyStudent(const StudentInfo& info);
    virtual ~DoubleFacultyStudent() {}

    ExamResult takeExam(const StudentInfo& student, const Exam& exam) const override;
    std::string getExamFormat(const Exam& exam) const override;
    void displayInfo() const override;

    std::string getFacultyName() const {
        return MexMatFaculty::getFacultyName() + " + " + HumanitiesFaculty::getFacultyName();
    }

    const StudentInfo& getStudentInfo() const { return studentInfo; }
};

#endif