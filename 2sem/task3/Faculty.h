#ifndef FACULTY_H
#define FACULTY_H

#include <iostream>
#include <string>
#include "Exam.h"
#include "ExamResults.h"
#include "StudentInfo.h"

class Faculty {
protected:
    std::string facultyName;
    int passingScore;

public:
    Faculty(const std::string& name, int passScore = 60) : facultyName(name), passingScore(passScore) {}

    virtual ~Faculty() {}

    virtual ExamResult takeExam(const StudentInfo& student, const Exam& exam) const = 0;
    virtual std::string getExamFormat(const Exam& exam) const = 0;

    virtual void displayInfo() const {
        std::cout << "  Факультет: " << facultyName << "\n";
        std::cout << "  Проходной балл: " << passingScore << "\n";
    }

    std::string getFacultyName() const {
        return facultyName;
    }

    int getPassingScore() const {
        return passingScore;
    }
};

#endif