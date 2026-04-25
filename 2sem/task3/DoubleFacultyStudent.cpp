#include "DoubleFacultyStudent.h"
#include <iostream>
#include <algorithm>

DoubleFacultyStudent::DoubleFacultyStudent(const StudentInfo& info)
    : Faculty("Межфакультетский студент", 70), MexMatFaculty(), HumanitiesFaculty(), studentInfo(info) {}

ExamResult DoubleFacultyStudent::takeExam(const StudentInfo& student, const Exam& exam) const {
    ExamResult mathResult = MexMatFaculty::takeExam(student, exam);
    ExamResult humanitiesResult = HumanitiesFaculty::takeExam(student, exam);

    int combinedScore = (mathResult.score + humanitiesResult.score) / 2;
    bool passed = mathResult.passed && humanitiesResult.passed;

    std::string grade;
    if (passed) {
        grade = (combinedScore >= 85) ? "Отлично" : ((combinedScore >= 70) ? "Хорошо" : "Удовлетворительно");
    } else {
        grade = "Неудовлетворительно";
    }

    return ExamResult(combinedScore, grade, passed);
}

std::string DoubleFacultyStudent::getExamFormat(const Exam& exam) const {
    return "КОМБИНИРОВАННЫЙ ЭКЗАМЕН:\n  " + MexMatFaculty::getExamFormat(exam) + "\n  " + HumanitiesFaculty::getExamFormat(exam);
}

void DoubleFacultyStudent::displayInfo() const {
    std::cout << "\n=== МЕЖФАКУЛЬТЕТСКИЙ СТУДЕНТ ===\n";
    studentInfo.display();
    std::cout << "Факультеты: " << getFacultyName() << "\n";
}