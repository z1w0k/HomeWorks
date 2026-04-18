#include "MexMatFaculty.h"
#include <algorithm>

MexMatFaculty::MexMatFaculty() : Faculty("Механико-математический факультет", 70), requiresCalculators(false) {}

ExamResult MexMatFaculty::takeExam(const StudentInfo& student, const Exam& exam) const {
    int finalScore = 0;
    
    if (student.solvedProblems > exam.problemCount) {
        finalScore = exam.maxScore;
    } else {
        finalScore = (student.solvedProblems * exam.maxScore) / exam.problemCount;
    }
    
    bool hasMathSkill = std::find(student.skills.begin(), student.skills.end(), "Топология") != student.skills.end() ||
                        std::find(student.skills.begin(), student.skills.end(), "Дифгем") != student.skills.end();
    
    if (hasMathSkill) {
        finalScore += 5;
    }
    
    finalScore += student.course * 2;
    
    finalScore += static_cast<int>(student.gpa * 3);
    
    if (student.usesCalculator && !requiresCalculators) {
        finalScore -= 25;
    }
    
    if (!student.usesCalculator && !requiresCalculators) {
        finalScore += 10;
    }
    
    finalScore = std::min(std::max(finalScore, 0), exam.maxScore);
    
    bool passed = finalScore >= passingScore;
    std::string grade;
    
    if (passed) {
        grade = (finalScore >= 85) ? "Отлично" : ((finalScore >= 70) ? "Хорошо" : "Удовлетворительно");
    } else {
        grade = "Неудовлетворительно";
    }
    
    return ExamResult(finalScore, grade, passed);
}

std::string MexMatFaculty::getExamFormat(const Exam& exam) const {
    return "Письменный экзамен: решение " + std::to_string(exam.problemCount) + ". Калькуляторы запрещены.";
}

void MexMatFaculty::displayInfo() const {
    Faculty::displayInfo();
}